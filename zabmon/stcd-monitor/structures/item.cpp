#include <stcd-monitor/structures/item.h>

using namespace zab::mon;

static const std::unordered_map<proto::ValueType, ValueType> g_valueTypeMap = {
    {proto::ValueType::VALUE_TYPE_INVALID, ValueType::Invalid},
    {proto::ValueType::VALUE_TYPE_NUMERIC_FLOAT, ValueType::NumericFloat},
    {proto::ValueType::VALUE_TYPE_CHARACTER, ValueType::Character},
    {proto::ValueType::VALUE_TYPE_LOG, ValueType::Log},
    {proto::ValueType::VALUE_TYPE_NUMERIC_UNSIGNED, ValueType::NumericUnsigned},
    {proto::ValueType::VALUE_TYPE_TEXT, ValueType::Text}};

ValueType Item::convertValueTypeFromProto(proto::ValueType valueType)
{
  auto it = g_valueTypeMap.find(valueType);
  if(it != g_valueTypeMap.end()) {
    return it->second;
  }
  return ValueType::Invalid;
}

proto::ValueType Item::convertValueTypeToProto(ValueType valueType)
{
  for(const auto &entry : g_valueTypeMap) {
    if(entry.second == valueType) {
      return entry.first;
    }
  }
  return proto::ValueType::VALUE_TYPE_INVALID;
}

static const std::unordered_map<proto::ItemType, ItemType> g_itemTypeMap = {
    {proto::ItemType::ITEM_TYPE_INVALID, ItemType::Invalid},
    {proto::ItemType::ITEM_TYPE_ZABBIX_AGENT, ItemType::ZabbixAgent},
    {proto::ItemType::ITEM_TYPE_SNMPV1_AGENT, ItemType::SnmpV1Agent},
    {proto::ItemType::ITEM_TYPE_TRAPPER, ItemType::Trapper},
    {proto::ItemType::ITEM_TYPE_SIMPLE_CHECK, ItemType::SimpleCheck},
    {proto::ItemType::ITEM_TYPE_SNMPV2_AGENT, ItemType::SnmpV2Agent},
    {proto::ItemType::ITEM_TYPE_INTERNAL, ItemType::Internal},
    {proto::ItemType::ITEM_TYPE_SNMPV3_AGENT, ItemType::SnmpV3Agent},
    {proto::ItemType::ITEM_TYPE_ZABBIX_AGENT_ACTIVE,
     ItemType::ZabbixAgentActive},
    {proto::ItemType::ITEM_TYPE_AGGREGATE, ItemType::Aggregate},
    {proto::ItemType::ITEM_TYPE_WEB_ELEMENT, ItemType::WebElement},
    {proto::ItemType::ITEM_TYPE_EXTERNAL_CHECK, ItemType::ExternalCheck},
    {proto::ItemType::ITEM_TYPE_DATABASE_MONITOR, ItemType::DatabaseMonitor},
    {proto::ItemType::ITEM_TYPE_IPMI_AGENT, ItemType::IpmAgent},
    {proto::ItemType::ITEM_TYPE_SSH_AGENT, ItemType::SshAgent},
    {proto::ItemType::ITEM_TYPE_TELNET_AGENT, ItemType::TelnetAgent},
    {proto::ItemType::ITEM_TYPE_CALCULATED, ItemType::Calculated},
    {proto::ItemType::ITEM_TYPE_JMX_AGENT, ItemType::JmxAgent},
    {proto::ItemType::ITEM_TYPE_SNMP_TRAP, ItemType::SnmpTrap},
    {proto::ItemType::ITEM_TYPE_DEPENDENT_ITEM, ItemType::DependentItem},
    {proto::ItemType::ITEM_TYPE_HTTP_AGENT, ItemType::HttpAgent}};

ItemType Item::convertItemTypeFromProto(proto::ItemType itemType)
{
  auto it = g_itemTypeMap.find(itemType);
  if(it != g_itemTypeMap.end()) {
    return it->second;
  }
  return ItemType::Invalid;
}

proto::ItemType Item::convertItemTypeToProto(ItemType itemType)
{
  for(const auto &entry : g_itemTypeMap) {
    if(entry.second == itemType) {
      return entry.first;
    }
  }
  return proto::ItemType::ITEM_TYPE_INVALID;
}

nlohmann::json Item::toJson(const Item &item)
{
  nlohmann::json params;

  params["name"] = item.name;

  params["key_"] = item.metricKey;

  params["hostid"] = item.parentId;

  params["type"] = static_cast<int32_t>(item.type);

  params["value_type"] = static_cast<int32_t>(item.valueType);
  params["delay"] = item.delay + "s";
  if(!item.interfaceId.empty()) {
    params["interfaceid"] = item.interfaceId;
  }
  return params;
}

Item Item::fromJson(const nlohmann::json &itemObj)
{
  Item item;
  item.name = itemObj.at("name").get<std::string>();
  item.id = itemObj.at("itemid").get<std::string>();
  item.metricKey = itemObj.at("key_").get<std::string>();
  item.type =
      static_cast<ItemType>(std::stoi(itemObj.at("type").get<std::string>()));
  item.valueType = static_cast<ValueType>(
      std::stoi(itemObj.at("value_type").get<std::string>()));
  item.delay = itemObj.at("delay").get<std::string>();
  for(const nlohmann::json &host : itemObj.at("hosts")) {
    item.parentId = host.at("hostid").get<std::string>();
  }
  return item;
}