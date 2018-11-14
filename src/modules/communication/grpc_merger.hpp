#pragma once

#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include "grpc_util.hpp"
#include "../../application.hpp"
#include "../../../include/thread_pool.hpp"
#include "protos/protobuf_merger.grpc.pb.h"
#include "protos/protobuf_signer.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using grpc::Channel;

using grpc::ClientContext;

using grpc_merger::MergerCommunication;
using grpc_merger::CheckAliveRequest;
using grpc_merger::CheckAliveReply;
using grpc_merger::MergerDataRequest;
using grpc_merger::MergerDataReply;

using grpc_signer::SignerCommunication;
using grpc_signer::SignerDataRequest;
using grpc_signer::SignerDataReply;

namespace gruut {

    enum class CallStatus {
        CREATE, PROCESS, FINISH
    };

    class MergerRpcServer {
    public:
        ~MergerRpcServer() {
            m_server_merger->Shutdown();
            m_cq_merger->Shutdown();

            m_server_signer->Shutdown();
            m_cq_signer->Shutdown();
        }

        void runMergerServ(char *port_for_merger) {
            std::string port_num(port_for_merger);
            std::string server_address("0.0.0.0:");
            server_address += port_num;

            ServerBuilder builder;
            builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

            builder.RegisterService(&m_service_merger);
            m_cq_merger = builder.AddCompletionQueue();
            m_server_merger = builder.BuildAndStart();
            std::cout << "Server listening on " << server_address << " for Merger" << std::endl;

            handleMergerRpcs();
        }

        void runSignerServ(char *port_for_signer, std::shared_ptr<std::queue<std::string>> merger_q) {
            m_merger_q = merger_q;
            std::string port_num(port_for_signer);
            std::string server_address("0.0.0.0:");
            server_address += port_num;

            ServerBuilder builder;
            builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

            builder.RegisterService(&m_service_signer);
            m_cq_signer = builder.AddCompletionQueue();
            m_server_signer = builder.BuildAndStart();
            std::cout << "Server listening on " << server_address << " for Signer" << std::endl;

            handleSignerRpcs();
        }

    private:
        class CallData {
        public:
            virtual void proceed() = 0;
        };

        class CheckAlive final : public CallData {
        public:

            CheckAlive(MergerCommunication::AsyncService *service, ServerCompletionQueue *cq)
                    : m_check_service(service), m_check_cq(cq), m_check_responder(&m_check_ctx),
                      m_check_status(CallStatus::CREATE) {

                proceed();
            }

            void proceed() {
                switch (m_check_status) {
                    case CallStatus::CREATE: {
                        m_check_status = CallStatus::PROCESS;
                        m_check_service->RequestcheckAlive(&m_check_ctx, &m_check_request, &m_check_responder,
                                                           m_check_cq, m_check_cq, this);
                    }
                        break;

                    case CallStatus::PROCESS: {
                        new CheckAlive(m_check_service, m_check_cq);

                        m_check_reply.set_message(true);
                        m_check_status = CallStatus::FINISH;
                        m_check_responder.Finish(m_check_reply, Status::OK, this);
                    }
                        break;

                    default: {
                        GPR_ASSERT(m_check_status == CallStatus::FINISH);
                        delete this;
                    }
                        break;
                }
            }

        private:
            MergerCommunication::AsyncService *m_check_service;
            ServerCompletionQueue *m_check_cq;

            ServerContext m_check_ctx;

            CheckAliveRequest m_check_request;
            CheckAliveReply m_check_reply;

            ServerAsyncResponseWriter<CheckAliveReply> m_check_responder;

            CallStatus m_check_status;
        };

        class PushData final : public CallData {
        public:
            PushData(MergerCommunication::AsyncService *service, ServerCompletionQueue *cq)
                    : m_push_service(service), m_push_cq(cq), m_push_responder(&m_push_ctx), m_push_status(CallStatus::CREATE) {

                proceed();
            }

            void proceed() {
                switch (m_push_status) {
                    case CallStatus::CREATE: {
                        m_push_status = CallStatus::PROCESS;
                        m_push_service->RequestpushData(&m_push_ctx, &m_push_request, &m_push_responder, m_push_cq,
                                                        m_push_cq, this);
                    }
                    break;

                    case CallStatus::PROCESS: {
                        new PushData(m_push_service, m_push_cq);

                        std::string raw_data = m_push_request.data();
                        if(!HeaderController::validateMessage(raw_data)) {
                            m_push_reply.set_checker(false);
                        }
                        else {
                            int json_size = HeaderController::getJsonSize(raw_data);
                            std::string compressed_data = HeaderController::detachHeader(raw_data);
                            std::string decompressed_data;

                            Compressor::decompressData(compressed_data, decompressed_data,json_size);

                            uint8_t message_type = HeaderController::getMessageType(raw_data);
                            Message msg;
                            msg.type = static_cast<InputMessageType>(message_type);
                            msg.data = nlohmann::json::parse(decompressed_data);

                            auto input_queue = Application::app().getInputQueue();
                            input_queue->push(msg);
                        }
                        m_push_status = CallStatus::FINISH;
                        m_push_responder.Finish(m_push_reply, Status::OK, this);
                    }
                    break;

                    default: {
                        GPR_ASSERT(m_push_status == CallStatus::FINISH);
                        delete this;
                    }
                    break;
                }
            }

        private:
            MergerCommunication::AsyncService *m_push_service;
            ServerCompletionQueue *m_push_cq;

            ServerContext m_push_ctx;

            MergerDataRequest m_push_request;
            MergerDataReply m_push_reply;

            ServerAsyncResponseWriter<MergerDataReply> m_push_responder;

            CallStatus m_push_status;
        };

        class PullRequest final : public CallData {
        public:
            PullRequest(SignerCommunication::AsyncService *service, ServerCompletionQueue *cq)
                    : m_pull_service(service), m_pull_cq(cq), m_pull_responder(&m_pull_ctx), m_pull_status(CallStatus ::CREATE) {

                proceed();
            }

            void proceed() {
                switch (m_pull_status) {
                    case CallStatus::CREATE: {
                        m_pull_status = CallStatus::PROCESS;
                        m_pull_service->RequestpullData(&m_pull_ctx, &m_pull_request, &m_pull_responder, m_pull_cq,
                                                        m_pull_cq, this);
                    }
                        break;

                    case CallStatus::PROCESS: {
                        new PullRequest(m_pull_service, m_pull_cq);

                        m_pull_reply.set_message("someData");
                        m_pull_status = CallStatus::FINISH;
                        m_pull_responder.Finish(m_pull_reply, Status::OK, this);
                    }
                        break;

                    default: {
                        GPR_ASSERT(m_pull_status == CallStatus::FINISH);
                        delete this;
                    }
                        break;
                }
            }

        private:
            SignerCommunication::AsyncService *m_pull_service;
            ServerCompletionQueue *m_pull_cq;

            ServerContext m_pull_ctx;

            SignerDataRequest m_pull_request;
            SignerDataReply m_pull_reply;

            ServerAsyncResponseWriter<SignerDataReply> m_pull_responder;

            CallStatus m_pull_status;
        };

        void handleMergerRpcs() {
            new CheckAlive(&m_service_merger, m_cq_merger.get());
            new PushData(&m_service_merger, m_cq_merger.get());
            void *tag;
            bool ok;
            while (true) {
                GPR_ASSERT(m_cq_merger->Next(&tag, &ok));
                GPR_ASSERT(ok);
                static_cast<CallData *>(tag)->proceed();
            }
        }

        void handleSignerRpcs() {
            new PullRequest(&m_service_signer, m_cq_signer.get());
            void *tag;
            bool ok;
            while (true) {
                GPR_ASSERT(m_cq_signer->Next(&tag, &ok));
                GPR_ASSERT(ok);
                static_cast<CallData *>(tag)->proceed();
            }
        }

        std::unique_ptr<ServerCompletionQueue> m_cq_merger;
        MergerCommunication::AsyncService m_service_merger;
        std::unique_ptr<Server> m_server_merger;

        std::unique_ptr<ServerCompletionQueue> m_cq_signer;
        SignerCommunication::AsyncService m_service_signer;
        std::unique_ptr<Server> m_server_signer;

        std::shared_ptr<std::queue<std::string>> m_merger_q;
    };

    class MergerRpcClient {
    public:
        MergerRpcClient(std::shared_ptr<Channel> channel)
                : m_stub(MergerCommunication::NewStub(channel)) {}

        bool checkAlive(bool checker) {
            CheckAliveRequest request;

            request.set_checker(true);

            CheckAliveReply reply;
            ClientContext context;

            Status status = m_stub->checkAlive(&context, request, &reply);
            if (status.ok()) {
                return reply.message();
            } else {
                std::cout << status.error_code() << ": " << status.error_message()
                          << std::endl;
                return false;
            }
        }

        bool pushData(std::string &compressed_data) {
            MergerDataRequest request;
            request.set_data(compressed_data);

            MergerDataReply reply;
            ClientContext context;
            Status status = m_stub->pushData(&context, request, &reply);

            if (status.ok())
                return reply.checker();
            else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
                return false;
            }
        }

    private:
        std::unique_ptr<MergerCommunication::Stub> m_stub;
    };

    class MessageHandler {
    public:
        void run() {
            m_pool = unique_ptr<ThreadPool>(new ThreadPool(5));
            auto output_queue = Application::app().getOutputQueue();

            //아래 if문을 포함하는 무한루프 필요.
            if (!output_queue->empty()) {
                Message msg = output_queue->front();

                std::string compressed_json;
                std::string json_dump = msg.data.dump();

                Compressor::compressData(json_dump, compressed_json);
                std::string header_added_data = HeaderController::attachHeader(json_dump, static_cast<uint8_t >(msg.type));

                switch(msg.type)
                {
                    case InputMessageType::BLOCK: {
                        sendDataToMerger(header_added_data);
                    }
                    break;
                    case InputMessageType::SIGNER: {
                        m_merger_q->push(header_added_data);
                    }
                    break;
                    default:{

                    }
                    break;
                }
                output_queue->pop();
            }
        }

    private:
        void sendDataToMerger(std::string &header_added_data) {

            std::unique_ptr<MergerRpcClient> merger_client
            (new MergerRpcClient(grpc::CreateChannel("ip and port",grpc::InsecureChannelCredentials())));

            m_pool->enqueue([&](){merger_client->pushData(header_added_data);});
        }
        std::unique_ptr<ThreadPool> m_pool;
        std::shared_ptr<std::queue<std::string>> m_merger_q;
    };
}