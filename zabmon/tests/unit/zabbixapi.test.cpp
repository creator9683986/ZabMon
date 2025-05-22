#include <stcd-monitor/api/zabbixapi.h>
#include <stcd-monitor/exceptions.h>
#include <stcd-monitor/structures/hostgroup.h>
#include <stcd-monitor/structures/itemvalues.h>
#include <stcd-monitor/structures/template.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <gtest/gtest.h>

class ZabbixApiTest : public ::testing::Test
{
 protected:
  std::shared_ptr<zab::mon::JsonRpcManager> m_jsonManager =
      std::make_shared<zab::mon::JsonRpcManager>(
          "http://zbx_front:8080/api_jsonrpc.php");
  std::shared_ptr<zab::mon::ZabbixApi> m_zabbix =
      std::make_shared<zab::mon::ZabbixApi>(m_jsonManager);
  std::shared_ptr<zab::mon::ZabbixApi> m_badzabbix =
      std::make_shared<zab::mon::ZabbixApi>(m_jsonManager);
  std::unordered_map<std::string, std::string> m_groupsIds;
  std::unordered_map<std::string, std::string> m_parentsIds;
  std::map<std::pair<std::string, std::string>, std::string> m_itemsIds;
  std::unordered_map<std::string, std::string> m_interfacesIds;
  // Время для того, чтобы zabbix сервер успел получить значения элементов
  // данных
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  std::chrono::seconds m_waitingTime = std::chrono::seconds(120);

  void SetUp() override
  {
    // Запуск сервера
    startServer();
    m_groupsIds = getGroupsIds();
    std::unordered_map<std::string, std::string> tempIds = getTemplatesIds();
    std::unordered_map<std::string, std::string> hostIds = getHostsIds();
    m_parentsIds.merge(hostIds);
    m_parentsIds.merge(tempIds);
    m_itemsIds = getItemsIds();
    m_interfacesIds = getInterfacesIds();
  }

  void TearDown() override { stopServer(); }
};

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetAuthTokenWithGoodParams)
{
  std::string ans = m_zabbix->getAuthToken("Admin", "zabbix");
  ASSERT_TRUE(!ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetAuthTokenWithBadLoginAndPassword)
{
  std::string ans = m_badzabbix->getAuthToken("Ad", "za");
  ASSERT_TRUE(ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetAllGroups)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");
  std::vector<zab::mon::HostGroup> ans = m_zabbix->getAllGroups(token);
  ASSERT_TRUE(!ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetAllTemplates)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  std::vector<std::string> filter;

  filter.emplace_back(m_groupsIds.at("Templates/Applications"));

  std::vector<zab::mon::Template> ans =
      m_zabbix->getAllTemplates(token, filter);

  ASSERT_TRUE(!ans.empty());

  std::vector<std::string> filter2;

  filter2.emplace_back(m_groupsIds.at("Hypervisors"));

  ans = m_zabbix->getAllTemplates(token, filter2);

  ASSERT_TRUE(ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetListItems)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  std::vector<std::string> filter;

  filter.emplace_back(m_parentsIds.at("Zabbix server"));

  std::vector<zab::mon::Item> ans =
      m_zabbix->getAllItems(token, {}, {}, filter);
  ASSERT_TRUE(!ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, GetAllHosts)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  std::vector<std::string> filter;

  filter.emplace_back(m_groupsIds.at("Zabbix servers"));

  std::vector<zab::mon::Host> ans = m_zabbix->getAllHosts(token, filter);

  ASSERT_TRUE(!ans.empty());

  std::vector<std::string> filter2;

  filter2.emplace_back(m_groupsIds.at("Hypervisors"));

  ans = m_zabbix->getAllHosts(token, filter2);

  ASSERT_TRUE(ans.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, getHistoryValue)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  zab::mon::Item item;

  item.id = m_itemsIds.at(
      std::make_pair(m_parentsIds.at("Zabbix server"), "Available memory"));

  item.valueType = ValueType::NumericUnsigned;
  std::this_thread::sleep_for(m_waitingTime);
  zab::mon::ItemValues ans = m_zabbix->getHistoryValue(item, 2, token);

  ASSERT_TRUE(!ans.values.empty());
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, DeleteHostWithWrongParametrs)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  EXPECT_THROW(!m_zabbix->deleteHost("sjdhsfjksdhjkfhsdkfhdsk", token),
               zab::mon::InvalidResponseError);
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, CreateAndDeleteHostGroup)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  zab::mon::HostGroup group{};
  group.name = "Test Group";
  std::string groupId = m_zabbix->createGroup(group, token);
  ASSERT_TRUE(!groupId.empty());
  ASSERT_TRUE(m_zabbix->deleteHostGroup(groupId, token));
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, CreateAndDeleteTemplate)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  zab::mon::Template temp{};
  zab::mon::HostGroup group{};
  group.name = "Test lkjlklkn";
  std::string groupId = m_zabbix->createGroup(group, token);
  ASSERT_TRUE(!groupId.empty());
  temp.name = "ZabbixApiTestTemplate";
  temp.groupsId.emplace_back(groupId);
  std::string tempId = m_zabbix->createTemplate(temp, token);
  ASSERT_TRUE(!tempId.empty());

  ASSERT_TRUE(m_zabbix->deleteTemplate(tempId, token));
  ASSERT_TRUE(m_zabbix->deleteHostGroup(groupId, token));
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, CreateAndDeleteItem)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  zab::mon::Item item{};
  item.name = "Test Item";
  item.metricKey = "icmpping";
  item.valueType = ValueType::NumericUnsigned;
  item.type = ItemType::SimpleCheck;
  item.delay = "30";
  item.parentId = m_parentsIds.at("Zabbix server");
  item.interfaceId = m_interfacesIds.at("Zabbix server");

  std::string itemId = m_zabbix->createItem(item, token);
  ASSERT_TRUE(!itemId.empty());

  ASSERT_TRUE(m_zabbix->deleteItem(itemId, token));
}

// NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
GTEST_TEST_F(ZabbixApiTest, HostCrudTest)
{
  std::string token = m_zabbix->getAuthToken("Admin", "zabbix");

  std::vector<std::string> groups;
  std::vector<std::string> tmpId;

  groups.emplace_back(m_groupsIds.at("Templates/Virtualization"));
  tmpId.emplace_back(m_parentsIds.at("Template App NTP Service"));

  std::vector<zab::mon::Interface> interfaces;
  interfaces.emplace_back("127.0.0.1", "", "10050", 1, 1, 1, "");
  zab::mon::Host host;
  host.name = "Test Host For Docker";
  host.id = "";
  host.groupsId = groups;
  host.templatesId = tmpId;
  host.interfaces = interfaces;

  std::string idHost = m_zabbix->createHost(host, token);

  ASSERT_TRUE(!idHost.empty());

  std::vector<zab::mon::Host> ans = m_zabbix->getAllHosts(token, {});
  bool flag = false;
  for(const auto &host : ans) {
    if(host.name == "Test Host For Docker") {
      flag = true;
      break;
    }
  }

  ASSERT_TRUE(flag);

  std::vector<std::string> groupsNew;

  groupsNew.emplace_back(m_groupsIds.at("Linux servers"));

  host.groupsId = groupsNew;
  host.id = idHost;
  std::string idHostUpdated = m_zabbix->updateHost(host, token);

  ASSERT_TRUE(idHost == idHostUpdated);
  ASSERT_TRUE(m_zabbix->deleteHost(idHost, token));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  const int rc = RUN_ALL_TESTS();

  return rc;
}