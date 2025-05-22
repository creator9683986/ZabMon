#include <structures/value.h>
#include <iostream>

using namespace zab::mon;

Value::Value(std::string stringValue, std::chrono::seconds clock,
             std::chrono::nanoseconds ns)
    : stringValue(std::move(stringValue))
    , clock(clock)
    , ns(ns)
{
}

Value Value::fromJson(const nlohmann::json& valObj)
{
  try {
    std::string valstr = valObj.at("value").get<std::string>();
    int64_t seconds = std::stoi(valObj.at("clock").get<std::string>());
    std::chrono::seconds clockOfVal(seconds);
    int64_t nanoseconds = std::stoi(valObj.at("ns").get<std::string>());
    std::chrono::nanoseconds ns(nanoseconds);
    return Value(valstr, clockOfVal, ns);
  } catch(const std::exception& e) {
    std::cerr<<"invalid argument "<<e.what();
    throw std::invalid_argument("Invalid argument");
  }
}