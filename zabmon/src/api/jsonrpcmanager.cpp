#include <api/jsonrpcmanager.h>
#include <exceptions.h>
#include <iostream>
#include <curl/curl.h>

using namespace zab::mon;

JsonRpcManager::JsonRpcManager(std::string url)
    : m_url(std::move(url))
{
}

size_t JsonRpcManager::writeCallback(void *contents, size_t size, size_t nmemb,
                                     std::string *userp)
{
  size_t totalSize = size * nmemb;
  userp->append(static_cast<char *>(contents), totalSize);
  return totalSize;
}

nlohmann::json JsonRpcManager::send(const std::string &method,
                                    const nlohmann::json &params,
                                    const std::string &token) const
{
  CURL *curl = curl_easy_init();
  CURLcode res = CURLE_OPERATION_TIMEDOUT;
  std::string readBuffer;
  if(curl == nullptr) {
    return readBuffer;
  }

  nlohmann::json jsonData;
  jsonData["jsonrpc"] = "2.0";
  jsonData["method"] = method;
  jsonData["params"] = params;
  if(!token.empty()) {
    jsonData["auth"] = token;
  }
  jsonData["id"] = 1;

  std::string jsonString = jsonData.dump();
  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    std::string s;
s = curl_easy_strerror(res);  
    throw ConnectionError("curl_easy_perform() failed: "+ s);
  }
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  nlohmann::json result;
  try {
    result = nlohmann::json::parse(readBuffer);
  } catch(const nlohmann::json::parse_error &e) {
     std::cerr << "JSON parse error: " << e.what();
  }
  if(result.contains("result")) {
    return result.at("result");
  }
  if(result.contains("error")) {
    std::cerr <<"Zabbix api error"<< result;
  }
  return "";
}
