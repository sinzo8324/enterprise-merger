// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: protobuf_MtoM.proto
#ifndef GRPC_protobuf_5fMtoM_2eproto__INCLUDED
#define GRPC_protobuf_5fMtoM_2eproto__INCLUDED

#include "protobuf_MtoM.pb.h"

#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace grpcMtoM {

class communication final {
 public:
  static constexpr char const* service_full_name() {
    return "grpcMtoM.communication";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status checkAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpcMtoM::CheckAliveReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>> AsynccheckAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>>(AsynccheckAliveRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>> PrepareAsynccheckAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>>(PrepareAsynccheckAliveRaw(context, request, cq));
    }
    virtual ::grpc::Status sendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpcMtoM::DataReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>> AsyncsendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>>(AsyncsendDataRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>> PrepareAsyncsendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>>(PrepareAsyncsendDataRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>* AsynccheckAliveRaw(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::CheckAliveReply>* PrepareAsynccheckAliveRaw(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>* AsyncsendDataRaw(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpcMtoM::DataReply>* PrepareAsyncsendDataRaw(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status checkAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpcMtoM::CheckAliveReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>> AsynccheckAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>>(AsynccheckAliveRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>> PrepareAsynccheckAlive(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>>(PrepareAsynccheckAliveRaw(context, request, cq));
    }
    ::grpc::Status sendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpcMtoM::DataReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>> AsyncsendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>>(AsyncsendDataRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>> PrepareAsyncsendData(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>>(PrepareAsyncsendDataRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>* AsynccheckAliveRaw(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::grpcMtoM::CheckAliveReply>* PrepareAsynccheckAliveRaw(::grpc::ClientContext* context, const ::grpcMtoM::CheckAliveRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>* AsyncsendDataRaw(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::grpcMtoM::DataReply>* PrepareAsyncsendDataRaw(::grpc::ClientContext* context, const ::grpcMtoM::DataRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_checkAlive_;
    const ::grpc::internal::RpcMethod rpcmethod_sendData_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status checkAlive(::grpc::ServerContext* context, const ::grpcMtoM::CheckAliveRequest* request, ::grpcMtoM::CheckAliveReply* response);
    virtual ::grpc::Status sendData(::grpc::ServerContext* context, const ::grpcMtoM::DataRequest* request, ::grpcMtoM::DataReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_checkAlive : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_checkAlive() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_checkAlive() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkAlive(::grpc::ServerContext* context, const ::grpcMtoM::CheckAliveRequest* request, ::grpcMtoM::CheckAliveReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcheckAlive(::grpc::ServerContext* context, ::grpcMtoM::CheckAliveRequest* request, ::grpc::ServerAsyncResponseWriter< ::grpcMtoM::CheckAliveReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_sendData : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_sendData() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_sendData() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status sendData(::grpc::ServerContext* context, const ::grpcMtoM::DataRequest* request, ::grpcMtoM::DataReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestsendData(::grpc::ServerContext* context, ::grpcMtoM::DataRequest* request, ::grpc::ServerAsyncResponseWriter< ::grpcMtoM::DataReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_checkAlive<WithAsyncMethod_sendData<Service > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_checkAlive : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_checkAlive() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_checkAlive() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkAlive(::grpc::ServerContext* context, const ::grpcMtoM::CheckAliveRequest* request, ::grpcMtoM::CheckAliveReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_sendData : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_sendData() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_sendData() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status sendData(::grpc::ServerContext* context, const ::grpcMtoM::DataRequest* request, ::grpcMtoM::DataReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_checkAlive : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_checkAlive() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_checkAlive() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkAlive(::grpc::ServerContext* context, const ::grpcMtoM::CheckAliveRequest* request, ::grpcMtoM::CheckAliveReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcheckAlive(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_sendData : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_sendData() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_sendData() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status sendData(::grpc::ServerContext* context, const ::grpcMtoM::DataRequest* request, ::grpcMtoM::DataReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestsendData(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_checkAlive : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_checkAlive() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler< ::grpcMtoM::CheckAliveRequest, ::grpcMtoM::CheckAliveReply>(std::bind(&WithStreamedUnaryMethod_checkAlive<BaseClass>::StreamedcheckAlive, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_checkAlive() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status checkAlive(::grpc::ServerContext* context, const ::grpcMtoM::CheckAliveRequest* request, ::grpcMtoM::CheckAliveReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedcheckAlive(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::grpcMtoM::CheckAliveRequest,::grpcMtoM::CheckAliveReply>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_sendData : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_sendData() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler< ::grpcMtoM::DataRequest, ::grpcMtoM::DataReply>(std::bind(&WithStreamedUnaryMethod_sendData<BaseClass>::StreamedsendData, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_sendData() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status sendData(::grpc::ServerContext* context, const ::grpcMtoM::DataRequest* request, ::grpcMtoM::DataReply* response) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedsendData(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::grpcMtoM::DataRequest,::grpcMtoM::DataReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_checkAlive<WithStreamedUnaryMethod_sendData<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_checkAlive<WithStreamedUnaryMethod_sendData<Service > > StreamedService;
};

}  // namespace grpcMtoM


#endif  // GRPC_protobuf_5fMtoM_2eproto__INCLUDED
