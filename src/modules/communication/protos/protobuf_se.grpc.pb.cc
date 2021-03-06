// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: protobuf_se.proto

#include "protobuf_se.pb.h"
#include "protobuf_se.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace grpc_se {

static const char* GruutSeService_method_names[] = {
  "/grpc_se.GruutSeService/seService",
};

std::unique_ptr< GruutSeService::Stub> GruutSeService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< GruutSeService::Stub> stub(new GruutSeService::Stub(channel));
  return stub;
}

GruutSeService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_seService_(GruutSeService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status GruutSeService::Stub::seService(::grpc::ClientContext* context, const ::grpc_se::Request& request, ::grpc_se::Reply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_seService_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::grpc_se::Reply>* GruutSeService::Stub::AsyncseServiceRaw(::grpc::ClientContext* context, const ::grpc_se::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::grpc_se::Reply>::Create(channel_.get(), cq, rpcmethod_seService_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::grpc_se::Reply>* GruutSeService::Stub::PrepareAsyncseServiceRaw(::grpc::ClientContext* context, const ::grpc_se::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::grpc_se::Reply>::Create(channel_.get(), cq, rpcmethod_seService_, context, request, false);
}

GruutSeService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      GruutSeService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< GruutSeService::Service, ::grpc_se::Request, ::grpc_se::Reply>(
          std::mem_fn(&GruutSeService::Service::seService), this)));
}

GruutSeService::Service::~Service() {
}

::grpc::Status GruutSeService::Service::seService(::grpc::ServerContext* context, const ::grpc_se::Request* request, ::grpc_se::Reply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace grpc_se

