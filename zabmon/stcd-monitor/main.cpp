#include <iostream>
#include <thread>

int main(int argc, char **argv)
{
    auto channel = grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials());
    JsonRpcManager("url zabbix");
    ZabbixApi(JsonRpcManager);
    Monitor(ZabbixApi, "Admin", "zabbix")
    
    
    
    Service service(Monitor);

    const std::string server_address("0.0.0.0:50051");
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    std::thread server_thread([&](){
        server->Wait();
    });
    server->Shutdown();
    server_thread.join();

    return 0;
}