#include <api/zabbixapi.h>
#include <monitor.h>
#include <service.h>
#include <structures/hostgroup.h>
#include <structures/itemvalues.h>
#include <structures/template.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <grpcpp/grpcpp.h>

#include <gtest/gtest.h>

#include "zabmon.grpc.pb.h"


class StcdMonitorTest : public ::testing::Test
{
 protected:

  std::chrono::seconds m_waitingTimeForData = std::chrono::seconds(180);
  std::unordered_map<std::string, std::string> m_groupsIds;
  std::unordered_map<std::string, std::string> m_parentsIds;
  std::map<std::pair<std::string, std::string>, std::string> m_itemsIds;
  std::unordered_map<std::string, std::string> m_interfacesIds;

  std::shared_ptr<zab::mon::Service> service_;
    std::unique_ptr<grpc::Server> server_;
    std::thread server_thread_;
    std::unique_ptr<zabmon::ZabMonService::Stub> stub_;
    const std::string server_address_ = "127.0.0.1:50051";

  std::shared_ptr<zab::mon::ZabbixApi>
  createClient()
  {
    std::shared_ptr<zab::mon::JsonRpcManager> jsonManager =
    std::make_shared<zab::mon::JsonRpcManager>(
        "http://localhost/zabbix/api_jsonrpc.php");
  std::shared_ptr<zab::mon::ZabbixApi> zabbix =
    std::make_shared<zab::mon::ZabbixApi>(jsonManager);
    return zabbix;
  }
  std::unique_ptr<zabmon::ZabMonService::Stub>
CreateMonitorStub() {
    auto channel = grpc::CreateChannel(server_address_,
        grpc::InsecureChannelCredentials());
    return zabmon::ZabMonService::NewStub(channel);
}
  std::unordered_map<std::string, std::string> getGroupsIds()
  {
    auto client =
        createClient();
        std::string token = client->getAuthToken("Admin", "zabbix");
    std::unordered_map<std::string, std::string> result;
    auto response = client->getAllGroups(token);
    for(const zab::mon::HostGroup &group : response) {
      result[group.name] = group.id;
    }
    return result;
  }
  
  std::unordered_map<std::string, std::string> getTemplatesIds()
  {
    auto client =
        createClient();
        std::string token = client->getAuthToken("Admin", "zabbix");
    std::unordered_map<std::string, std::string> result;
    auto response = client->getAllTemplates(token, {});
    for(const zab::mon::Template &temp : response) {
      result[temp.name] = temp.id;
    }
    return result;
  }
  
  std::unordered_map<std::string, std::string> getHostsIds()
  {
    auto client =
        createClient();
        std::string token = client->getAuthToken("Admin", "zabbix");

    std::unordered_map<std::string, std::string> result;
    auto response = client->getAllHosts(token, {});
  
    for(const zab::mon::Host &host : response) {
      result[host.name] = host.id;
    }
    return result;
  }
  
  std::map<std::pair<std::string, std::string>, std::string> getItemsIds()
  {
    auto client =
        createClient();
        std::string token = client->getAuthToken("Admin", "zabbix");

    std::map<std::pair<std::string, std::string>, std::string> result;
    auto response = client->getAllItems(token, {}, {}, {});
    for(const zab::mon::Item &item : response) {
      result.emplace(std::make_pair(item.parentId, item.name), item.id);
    }
    return result;
  }
  
  std::unordered_map<std::string, std::string> getInterfacesIds()
  {
    auto client =
        createClient();
        std::string token = client->getAuthToken("Admin", "zabbix");

    std::unordered_map<std::string, std::string> result;
    auto response = client->getAllHosts(token, {});
    for(const zab::mon::Host &host : response) {
      result[host.name] = host.interfaces.at(0).id;
    }
    return result;
  }

  void SetUp() override {
    auto jsonRpc = std::make_shared<zab::mon::JsonRpcManager>("http://localhost/zabbix/api_jsonrpc.php");
    auto zabbix  = std::make_shared<zab::mon::ZabbixApi>(jsonRpc);
    auto monitor = std::make_shared<zab::mon::Monitor>(zabbix, "Admin", "zabbix");
    service_ = std::make_shared<zab::mon::Service>(monitor);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
    builder.RegisterService(service_.get());
    server_ = builder.BuildAndStart();
    ASSERT_NE(server_, nullptr);

    server_thread_ = std::thread([this]() {
        server_->Wait();
    });

    auto channel = grpc::CreateChannel(server_address_, grpc::InsecureChannelCredentials());
    stub_ = zabmon::ZabMonService::NewStub(channel);
    m_groupsIds = getGroupsIds();
    std::unordered_map<std::string, std::string> tempIds = getTemplatesIds();
    std::unordered_map<std::string, std::string> hostIds = getHostsIds();
    m_parentsIds.merge(hostIds);
    m_parentsIds.merge(tempIds);
    m_itemsIds = getItemsIds();
    m_interfacesIds = getInterfacesIds();
}

void TearDown() override {
    server_->Shutdown();
    if (server_thread_.joinable()) server_thread_.join();
}
};

const auto g_checkHost =
    [](const std::string &hostName,
       const std::unique_ptr<zabmon::ZabMonService::Stub>
           &client) {
      zabmon::ListHostsResponse resp;
      grpc::ClientContext ctx;
      auto requestCheck = zabmon::ListHostsRequest();
      grpc::Status responseCheck =
          client->listHosts(&ctx, requestCheck,&resp);
      bool flag = false;
      for(const auto &host : resp.hosts()) {
        if(host.name() == hostName) {
          flag = true;
          break;
        }
      }
      return flag;
    };

const auto g_checkTemplate =
    [](const std::string &tempName,
       const std::unique_ptr<zabmon::ZabMonService::Stub>
           &client) {
      auto requestCheck = zabmon::ListTemplatesRequest();
      zabmon::ListTemplatesResponse resp;
      grpc::ClientContext ctx;
      grpc::Status responseCheck =
          client->listTemplates(&ctx, requestCheck,&resp);
      bool flag = false;
      for(const auto &temp : resp.templates()) {
        if(temp.name() == tempName) {
          flag = true;
          break;
        }
      }
      return flag;
    };

const auto g_checkGroup =
    [](const std::string &groupName,
       const std::unique_ptr<zabmon::ZabMonService::Stub>
           &client) {
      auto requestCheck = zabmon::ListGroupsRequest();
      zabmon::ListGroupsResponse resp;
      grpc::ClientContext ctx;
      grpc::Status responseCheck =
          client->listGroups(&ctx, requestCheck,&resp);
      bool flag = false;
      for(const auto &gr : resp.groups()) {
        if(gr.name() == groupName) {
          flag = true;
          break;
        }
      }
      return flag;
    };

const auto g_checkItem =
    [](const std::string &itemName, const std::string &hostId,
       const std::unique_ptr<zabmon::ZabMonService::Stub>
           &client) {
      auto requestCheck = zabmon::ListItemsRequest();
      zabmon::ListItemsResponse resp;
      grpc::ClientContext ctx;
      grpc::Status responseCheck =
          client->listItems(&ctx, requestCheck,&resp);
      bool flag = false;
      for(const auto &item : resp.items()) {
        if(item.name() == itemName && item.parent_id() == hostId) {
          flag = true;
          break;
        }
      }
      return flag;
    };


GTEST_TEST_F(StcdMonitorTest, GetListGroups)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto request = zabmon::ListGroupsRequest();
    zabmon::ListGroupsResponse resp;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->listGroups(&ctx, request,&resp);
    ASSERT_TRUE(resp.groups_size() == 18);
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, getListTemplates)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto request = zabmon::ListTemplatesRequest();
    auto *gr = request.mutable_groups_ids()->Add();
    gr->append(m_groupsIds.at("Templates/Telephony"));
    zabmon::ListTemplatesResponse resp;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->listTemplates(&ctx, request,&resp);
    ASSERT_TRUE(resp.templates_size() == 1);
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, getListItems)
{
  constexpr int itemsHostFilter = 121;
  constexpr int itemsTemplateFilter = 1;
  constexpr int itemsGroupFilter = 0;

  std::unique_ptr<zabmon::ZabMonService::Stub> client =
      StcdMonitorTest::CreateMonitorStub();

  const auto testImpl =
      [](const zabmon::ListItemsRequest &request, int32_t size,
         const std::unique_ptr<zabmon::ZabMonService::Stub> &client) {
        try {
            zabmon::ListItemsResponse resp;
            grpc::ClientContext ctx;
            grpc::Status responseCheck =
                client->listItems(&ctx, request,&resp);
         
          ASSERT_EQ(resp.items_size(), size);
        } catch(const std::exception &e) {
          ASSERT_TRUE(false);
        }
      };

  auto request = zabmon::ListItemsRequest();
  request.add_hosts_ids(m_parentsIds.at("Zabbix server"));
  testImpl(request, itemsHostFilter, client);

}


GTEST_TEST_F(StcdMonitorTest, getListItemsValues)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();

    auto request = zabmon::ListItemsValuesRequest();
    request.set_limit(2);
    auto *item = request.mutable_items()->Add();
    std::string itemId = m_itemsIds.at(
        std::make_pair(m_parentsIds.at("Zabbix server"), "Linux: Available memory"));
    item->set_id(itemId);
    item->set_value_type(
        zabmon::ValueType::VALUE_TYPE_NUMERIC_UNSIGNED);

    zabmon::ListItemsValuesResponse resp;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->listItemsValues(&ctx, request,&resp);
    const auto &itemValues = resp.values(0);
    auto valuesSize = itemValues.values_size();
    std::string firstValue = resp.values(0).values(0).value();

    ASSERT_TRUE(valuesSize == 2);
    ASSERT_TRUE(firstValue != "0" && !firstValue.empty());
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, GetListHosts)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto request = zabmon::ListHostsRequest();
    auto *filter = request.mutable_groups_ids()->Add();
    filter->append(m_groupsIds.at("Zabbix servers"));
    zabmon::ListHostsResponse resp;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->listHosts(&ctx, request,&resp);
    bool flag = false;
    for(const auto &host : resp.hosts()) {
      if(host.name() == "Zabbix server") {
        flag = true;
        break;
      }
    }
    ASSERT_TRUE(flag);

    auto requestBad = zabmon::ListHostsRequest();
    auto *filterBad = requestBad.mutable_groups_ids()->Add();
    filterBad->append(m_groupsIds.at("Templates/Applications"));
    zabmon::ListHostsResponse responseBad;
    grpc::ClientContext ctxBad;
    responseCheck =
        client->listHosts(&ctxBad, requestBad,&responseBad);
    bool flagBad = false;
    for(const auto &host : responseBad.hosts()) {
      if(host.name() == "Zabbix server") {
        flagBad = true;
        break;
      }
    }
    ASSERT_FALSE(flagBad);
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, CRUD)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();

    const int port = 10050;
    auto requestCR = zabmon::CreateHostRequest();
    auto *host = requestCR.mutable_host();
    host->set_name("TestHostForDocker");
    auto *gr = host->mutable_groups_ids()->Add();
    gr->append(m_groupsIds.at("Templates/Virtualization"));
    auto *tmp = host->mutable_templates_ids()->Add();
    tmp->append(m_parentsIds.at("NNTP Service"));
    auto *inter = host->mutable_interfaces()->Add();
    inter->set_dns("sdfsdfsdfsdfsd.ru");
    inter->set_port(port);

    zabmon::Host responseCR;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->createHost(&ctx, requestCR,&responseCR);
    ASSERT_TRUE(responseCR.descriptor()->DebugString() ==
                host->descriptor()->DebugString());
    ASSERT_TRUE(g_checkHost("TestHostForDocker", client));

    auto requestUP = zabmon::UpdateHostRequest();
    requestUP.mutable_host()->CopyFrom(requestCR.host());
    requestUP.mutable_host()->set_id(responseCR.id());
    auto *group = requestUP.mutable_host()->mutable_groups_ids();
    group->Clear();
    group->Add()->append(m_groupsIds.at("Linux servers"));
    zabmon::Host responseUP;
    grpc::ClientContext ctxUp;
    responseCheck =
        client->updateHost(&ctxUp, requestUP,&responseUP);
    ASSERT_TRUE(responseUP.descriptor()->DebugString() ==
                requestUP.mutable_host()->descriptor()->DebugString());
    ASSERT_TRUE(g_checkHost("TestHostForDocker", client));

    auto requestDL = zabmon::DeleteHostRequest();
    requestDL.set_id(responseUP.id());
    ::google::protobuf::Empty responseDL;
    grpc::ClientContext ctxDl;
    responseCheck =
        client->deleteHost(&ctxDl, requestDL,&responseDL);
    ASSERT_TRUE(!g_checkHost("TestHostForDocker", client));
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, CreateAndDeleteTemplate)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto requestCR = zabmon::CreateTemplateRequest();
    auto *temp = requestCR.mutable_template_();
    temp->set_name("ApiTestTemplate");
    auto *gr = temp->mutable_groups_ids()->Add();
    gr->append(m_groupsIds.at("Virtual machines"));

    zabmon::Template responseCR;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->createTemplate(&ctx, requestCR,&responseCR);
    ASSERT_TRUE(responseCR.descriptor()->DebugString() ==
                temp->descriptor()->DebugString());

    auto requestDL = zabmon::DeleteTemplateRequest();
    requestDL.set_id(responseCR.id());
    ::google::protobuf::Empty responseDL;
    grpc::ClientContext ctxDl;
    responseCheck =
        client->deleteTemplate(&ctxDl, requestDL,&responseDL);
    ASSERT_TRUE(!g_checkTemplate("ApiTestTemplate", client));
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}


GTEST_TEST_F(StcdMonitorTest, CreateAndDeleteGroup)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto requestCR = zabmon::CreateGroupRequest();
    auto *gr = requestCR.mutable_group();
    gr->set_name("TestGroup");

    zabmon::Group responseCR;
    grpc::ClientContext ctx;
    grpc::Status responseCheck =
        client->createGroup(&ctx, requestCR,&responseCR);
    ASSERT_TRUE(responseCR.descriptor()->DebugString() ==
                gr->descriptor()->DebugString());

    auto requestDL = zabmon::DeleteGroupRequest();
    requestDL.set_id(responseCR.id());
    ::google::protobuf::Empty responseDL;
    grpc::ClientContext ctxDl;
     responseCheck =
        client->deleteGroup(&ctxDl, requestDL,&responseDL);
    ASSERT_TRUE(!g_checkGroup("TestGroup", client));
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}

GTEST_TEST_F(StcdMonitorTest, CreateAndDeleteItem)
{
  try {
    std::unique_ptr<zabmon::ZabMonService::Stub> client =
        StcdMonitorTest::CreateMonitorStub();
    auto requestCR = zabmon::CreateItemRequest();
    auto *item = requestCR.mutable_item();

    item->set_delay("30");
    item->set_metric_key("icmpping");
    item->set_name("Test Item");
    item->set_parent_id(m_parentsIds.at("Zabbix server"));
    item->set_type(zabmon::ItemType::ITEM_TYPE_SIMPLE_CHECK);
    item->set_value_type(
        zabmon::ValueType::VALUE_TYPE_NUMERIC_UNSIGNED);
    item->set_interface_id(m_interfacesIds.at("Zabbix server"));

    zabmon::Item responseCR;
    grpc::ClientContext ctx;
    auto responseCheck =
        client->createItem(&ctx, requestCR,&responseCR);
    ASSERT_TRUE(responseCR.descriptor()->DebugString() ==
                item->descriptor()->DebugString());

    auto requestDL = zabmon::DeleteItemRequest();

    requestDL.set_id(responseCR.id());

    ::google::protobuf::Empty responseDL;
    grpc::ClientContext ctxDl;

    responseCheck =
        client->deleteItem(&ctxDl, requestDL,&responseDL);
    ASSERT_TRUE(!g_checkItem("Test Item", "Zabbix server", client));
  } catch(const std::exception &e) {
    ASSERT_TRUE(false);
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  const int rc = RUN_ALL_TESTS();

  return rc;
}