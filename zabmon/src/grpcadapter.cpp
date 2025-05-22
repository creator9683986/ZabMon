#include <exceptions.h>
#include <grpcadapter.h>

using namespace zab::mon;

Host GRPCAdapter::toHost(const zabmon::Host& host)
{
  Host result;

  result.name = host.name();
  result.id = host.id();
  ::google::protobuf::RepeatedPtrField<zabmon::Interface>
      interfaces = host.interfaces();
  for(const auto& interface : interfaces) {
    std::string ip = interface.ip();
    std::string dns = interface.dns();
    std::string port = std::to_string(interface.port());
    result.interfaces.emplace_back(ip, dns, port, 1, 1, 0, "");
  }

  for(const std::string& groupId : host.groups_ids()) {
    result.groupsId.push_back(groupId);
  }

  for(const std::string& tempId : host.templates_ids()) {
    result.templatesId.emplace_back(tempId);
  }

  return result;
}

Item GRPCAdapter::toItem(const zabmon::Item& item)
{
  Item result;

  result.interfaceId = item.interface_id();
  result.parentId = item.parent_id();
  result.name = item.name();

  result.metricKey = item.metric_key();

  result.type = Item::convertItemTypeFromProto(item.type());

  result.valueType = Item::convertValueTypeFromProto(item.value_type());
  result.id = item.id();
  result.delay = item.delay();

  return result;
}

void GRPCAdapter::fromGroups(const std::vector<HostGroup>& allGroups,
                              zabmon::ListGroupsResponse* result)
{
  for(const HostGroup& group : allGroups) {
    zabmon::Group* grouppt = result->add_groups();
    grouppt->set_name(group.name);
    grouppt->set_id(group.id);

    for(const std::string& tempId : group.templatesIds) {
      grouppt->add_templates_ids(tempId);
    }
  }
}

void GRPCAdapter::fromValues(const std::vector<ItemValues>& itemsValues,
                              zabmon::ListItemsValuesResponse* result)
{
  for(const ItemValues& itemValue : itemsValues) {
    zabmon::ItemValues* itemValuept = result->add_values();
    for(const Value& val : itemValue.values) {
      zabmon::Value* valuept = itemValuept->add_values();
      google::protobuf::Timestamp* timestamp = valuept->mutable_timestamp();
      int64_t seconds = val.clock.count();
      int nanoseconds = static_cast<int>(val.ns.count());
      timestamp->set_seconds(seconds);
      timestamp->set_nanos(nanoseconds);
      valuept->set_value(val.stringValue);
    }
    fromItem(itemValue.item, itemValuept->mutable_item());
  }
}

void GRPCAdapter::fromGroup(const HostGroup& hostGroup,
                             zabmon::Group* result)
{
  result->set_name(hostGroup.name);
  result->set_id(hostGroup.id);
  for(const std::string& tempId : hostGroup.templatesIds) {
    result->add_templates_ids(tempId);
  }
}

HostGroup GRPCAdapter::toGroup(const zabmon::Group& group)
{
  HostGroup result;
  result.name = group.name();
  result.id = group.id();
  for(const std::string& tempId : group.templates_ids()) {
    result.templatesIds.push_back(tempId);
  }
  return result;
}
Template GRPCAdapter::toTemplate(const zabmon::Template& temp)
{
  Template result;
  result.name = temp.name();
  result.id = temp.id();
  for(const std::string& groupId : temp.groups_ids()) {
    result.groupsId.push_back(groupId);
  }
  return result;
}

void GRPCAdapter::fromHost(const Host& host,  zabmon::Host* result)
{
  result->set_name(host.name);
  // interfaces
  for(const Interface& interface : host.interfaces) {
    fromInterface(interface, result->add_interfaces());
  }

  // groups
  for(const std::string& group : host.groupsId) {
    result->add_groups_ids(group);
  }

  // templates
  for(const std::string& temp : host.templatesId) {
    result->add_templates_ids(temp);
  }
  result->set_id(host.id);
}

void GRPCAdapter::fromItem(const Item& item,  zabmon::Item* result)
{
  result->set_name(item.name);
  result->set_parent_id(item.parentId);
  result->set_id(item.id);
  result->set_interface_id(item.interfaceId);

  result->set_metric_key(item.metricKey);

  result->set_type(Item::convertItemTypeToProto(item.type));
  result->set_value_type(Item::convertValueTypeToProto(item.valueType));
  result->set_delay(item.delay);
}

void GRPCAdapter::fromTemplate(const Template& temp,
                                zabmon::Template* result)
{
  result->set_name(temp.name);
  result->set_id(temp.id);
  for(const std::string& groupId : temp.groupsId) {
    result->add_groups_ids(groupId);
  }
}

void GRPCAdapter::fromInterface(const Interface& interface,
                                 zabmon::Interface* result)
{
  result->set_ip(interface.ip);
  result->set_port(std::stoi(interface.port));
  result->set_dns(interface.dns);
  if(!interface.id.empty()) {
    result->set_id(interface.id);
  }
}