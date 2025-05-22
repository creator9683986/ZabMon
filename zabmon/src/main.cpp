#include <iostream>
#include <thread>
#include <grpcpp/grpcpp.h>
#include <service.h>
#include <api/jsonrpcmanager.h> 

int main(int argc, char **argv)
{
    auto channel = grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials());
    auto jsonRpc = std::make_shared<zab::mon::JsonRpcManager>("http://localhost/api_jsonrpc.php");
    auto zabbix = std::make_shared<zab::mon::ZabbixApi>(jsonRpc);
    auto monitor = std::make_shared<zab::mon::Monitor>(zabbix, "Admin", "zabbix");
    
    
    
    zab::mon::Service service(monitor);

    const std::string server_address("0.0.0.0:50051");
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();

    return 0;
}