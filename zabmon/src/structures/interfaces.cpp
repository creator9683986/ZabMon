#include <structures/interfaces.h>

using namespace zab::mon;

Interface::Interface(std::string ip, std::string dns, std::string port,
                     int type, int main, int useip, std::string id)
    : ip(std::move(ip))
    , dns(std::move(dns))
    , port(std::move(port))
    , type(type)
    , main(main)
    , useip(useip)
    , id(std::move(id))

{
}

Interface Interface::fromJson(const nlohmann::json &interfaceObj)
{
  std::string ip = interfaceObj.at("ip").get<std::string>();
  std::string port = interfaceObj.at("port").get<std::string>();
  std::string dns = interfaceObj.at("dns").get<std::string>();
  int type = std::stoi(interfaceObj.at("type").get<std::string>());
  int main = std::stoi(interfaceObj.at("main").get<std::string>());
  int useip = std::stoi(interfaceObj.at("useip").get<std::string>());
  std::string id = interfaceObj.at("interfaceid").get<std::string>();
  return Interface(ip, dns, port, type, main, useip, id);
}
