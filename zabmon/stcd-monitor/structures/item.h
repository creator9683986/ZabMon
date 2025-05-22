#ifndef ITEM_H
#define ITEM_H

#include <nlohmann/json.hpp>
#include <stcd-monitor-proto/messages.pb.h>

enum class ItemType {
  Invalid = -1,           // ITEM_TYPE_INVALID
  ZabbixAgent = 0,        // ITEM_TYPE_ZABBIX_AGENT
  SnmpV1Agent = 1,        // ITEM_TYPE_SNMPV1_AGENT
  Trapper = 2,            // ITEM_TYPE_TRAPPER
  SimpleCheck = 3,        // ITEM_TYPE_SIMPLE_CHECK
  SnmpV2Agent = 4,        // ITEM_TYPE_SNMPV2_AGENT
  Internal = 5,           // ITEM_TYPE_INTERNAL
  SnmpV3Agent = 6,        // ITEM_TYPE_SNMPV3_AGENT
  ZabbixAgentActive = 7,  // ITEM_TYPE_ZABBIX_AGENT_ACTIVE
  Aggregate = 8,          // ITEM_TYPE_AGGREGATE
  WebElement = 9,         // ITEM_TYPE_WEB_ELEMENT
  ExternalCheck = 10,     // ITEM_TYPE_EXTERNAL_CHECK
  DatabaseMonitor = 11,   // ITEM_TYPE_DATABASE_MONITOR
  IpmAgent = 12,          // ITEM_TYPE_IPMI_AGENT
  SshAgent = 13,          // ITEM_TYPE_SSH_AGENT
  TelnetAgent = 14,       // ITEM_TYPE_TELNET_AGENT
  Calculated = 15,        // ITEM_TYPE_CALCULATED
  JmxAgent = 16,          // ITEM_TYPE_JMX_AGENT
  SnmpTrap = 17,          // ITEM_TYPE_SNMP_TRAP
  DependentItem = 18,     // ITEM_TYPE_DEPENDENT_ITEM
  HttpAgent = 19          // ITEM_TYPE_HTTP_AGENT
};

enum class ValueType {
  Invalid = -1,         // VALUE_TYPE_INVALID
  NumericFloat = 0,     // VALUE_TYPE_NUMERIC_FLOAT
  Character = 1,        // VALUE_TYPE_CHARACTER
  Log = 2,              // VALUE_TYPE_LOG
  NumericUnsigned = 3,  // VALUE_TYPE_NUMERIC_UNSIGNED
  Text = 4              // VALUE_TYPE_TEXT
};

namespace zab::mon
{
/// @brief Структура элемента данных
struct Item {
  /// @brief ключ элемента данных
  std::string metricKey;
  /// @brief тип итема
  ItemType type = ItemType::Invalid;
  /// @brief тип значения
  ValueType valueType = ValueType::Invalid;
  /// @brief период опроса
  std::string delay;
  /// @brief id интерфейса
  std::string interfaceId;
  /// @brief id итема
  std::string id;
  /// @brief id родителя
  std::string parentId;
  /// @brief имя итема
  std::string name;
  /// @brief Конструктор
  /// @param metricKey ключ элемента данных
  /// @param type тип итема
  /// @param valueType тип значения
  /// @param delay период опроса
  explicit Item(std::string metricKey, ItemType type, ValueType valueType,
                std::string delay);
  explicit Item() = default;
  static nlohmann::json toJson(const Item &item);
  static Item fromJson(const nlohmann::json &itemObj);
  static ValueType convertValueTypeFromProto(proto::ValueType valueType);
  static proto::ValueType convertValueTypeToProto(ValueType valueType);
  static ItemType convertItemTypeFromProto(proto::ItemType itemType);
  static proto::ItemType convertItemTypeToProto(ItemType itemType);
};
}  // namespace zab::mon

#endif  // ITEM_H