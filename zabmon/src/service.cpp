
#include <exceptions.h>
#include <grpcadapter.h>
#include <service.h>
#include <structures/host.h>
#include <structures/interfaces.h>
#include <structures/itemvalues.h>

#include <iostream>
#include <thread>

using namespace zab::mon;

Service::Service(std::shared_ptr<Monitor> monitor)
    : m_monitor(std::move(monitor))
{
}

::grpc::Status Service::createHost(::grpc::ServerContext* /*context*/,
                                   const zabmon::CreateHostRequest* request,
                                   zabmon::Host* response)
{
  Host host;
  try {
    const zabmon::Host& hostFromgRpc = request->host();
    if(hostFromgRpc.interfaces_size() != 1) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,"Host must have exactly one interface");
    }
    host = GRPCAdapter::toHost(hostFromgRpc);
    host.id = m_monitor->createHost(host);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json, " << e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json");
  } catch(const InvalidResponseError& e) {
   std::cerr<<"Invalid response from zabbix" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,"Invalid response from zabbix");
  } catch(const std::exception& e) {
   std::cerr<<"Cant create Host";
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,"Can't create Host, invalid arguments");
  }
  GRPCAdapter::fromHost(host, response);
  return ::grpc::Status::OK;
}

::grpc::Status Service::deleteHost(
    ::grpc::ServerContext* /*context*/,
    const zabmon::DeleteHostRequest* request,
    ::google::protobuf::Empty* /*response*/)
{
  try {
    if(!m_monitor->deleteHost(request->id())) {
      return grpc::Status(grpc::StatusCode::NOT_FOUND,
                          "Error while delete");
    }
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant delete Host" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't delete Host, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::deleteGroup(
    ::grpc::ServerContext* /*context*/,
    const zabmon::DeleteGroupRequest* request,
    ::google::protobuf::Empty* /*response*/)
{
  try {
    if(!m_monitor->deleteHostGroup(request->id())) {
      return grpc::Status(grpc::StatusCode::NOT_FOUND,
                          "Error while delete");
    }
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant create Host" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't delete Group, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::deleteTemplate(
    ::grpc::ServerContext* /*context*/,
    const zabmon::DeleteTemplateRequest* request,
    ::google::protobuf::Empty* /*response*/)
{
  try {
    if(!m_monitor->deleteTemplate(request->id())) {
      return grpc::Status(grpc::StatusCode::NOT_FOUND,
                          "Error while delete");
    }
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json,");
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant create Host" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't delete Template, invalid arguments");
  }

  return ::grpc::Status::OK;
}

::grpc::Status Service::deleteItem(
    ::grpc::ServerContext* /*context*/,
    const zabmon::DeleteItemRequest* request,
    ::google::protobuf::Empty* /*response*/)
{
  try {
    if(!m_monitor->deleteItem(request->id())) {
      return grpc::Status(grpc::StatusCode::NOT_FOUND,
                          "Error while delete");
    }
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant delete Item" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't delete Item, invalid arguments");
  }

  return ::grpc::Status::OK;
}

::grpc::Status Service::updateHost(
    ::grpc::ServerContext* /*context*/,
    const zabmon::UpdateHostRequest* request,
    zabmon::Host* response)
{
  Host host;
  try {
    const zabmon::Host& hostFromgRpc = request->host();
    host = GRPCAdapter::toHost(hostFromgRpc);
    host.id = m_monitor->updateHost(host);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant update Host" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't update Host, invalid arguments");
  }

  GRPCAdapter::fromHost(host, response);
  return ::grpc::Status::OK;
}

::grpc::Status Service::listHosts(
    ::grpc::ServerContext* /*context*/,
    const zabmon::ListHostsRequest* request,
    zabmon::ListHostsResponse* response)
{
  try {
    std::vector<std::string> filter;
    for(const std::string& groupid : request->groups_ids()) {
      filter.push_back(groupid);
    }

    std::vector<Host> allHosts = m_monitor->getAllHosts(filter);
    for(const Host& host : allHosts) {
      GRPCAdapter::fromHost(host, response->add_hosts());
      response->set_next_page_token("");
    }
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json,");
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix,");
  } catch(const std::exception& e) {
    std::cerr<<"Cant list Hosts" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't list Hosts, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::listGroups(
    ::grpc::ServerContext* /*context*/,
    const zabmon::ListGroupsRequest* /*request*/,
    zabmon::ListGroupsResponse* response)
{
  try {
    std::vector<HostGroup> hostGroups = m_monitor->getAllGroups();
    GRPCAdapter::fromGroups(hostGroups, response);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant list Groups" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't list Groups, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::listTemplates(
    ::grpc::ServerContext* /*context*/,
    const zabmon::ListTemplatesRequest* request,
    zabmon::ListTemplatesResponse* response)
{
  try {
    std::vector<std::string> filter;
    for(const std::string& groupId : request->groups_ids()) {
      filter.push_back(groupId);
    }
    std::vector<Template> temps = m_monitor->getAllTemplates(filter);
    for(const Template& temp : temps) {
      std::unique_ptr<zabmon::Template> templateResult =
          std::make_unique<zabmon::Template>();
      GRPCAdapter::fromTemplate(temp, templateResult.get());
      response->mutable_templates()->AddAllocated(templateResult.release());
    }

  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix,");
  } catch(const std::exception& e) {
    std::cerr<<"Cant list Templates" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't list Templates, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::listItems(
    ::grpc::ServerContext* /*context*/,
    const zabmon::ListItemsRequest* request,
    zabmon::ListItemsResponse* response)
{
  try {
    std::vector<std::string> filterGroups;
    for(const std::string& groupId : request->groups_ids()) {
      filterGroups.push_back(groupId);
    }

    std::vector<std::string> filterTemplates;
    for(const std::string& tempId : request->templates_ids()) {
      filterTemplates.push_back(tempId);
    }

    std::vector<std::string> filterHosts;
    for(const std::string& hostId : request->hosts_ids()) {
      filterHosts.push_back(hostId);
    }

    std::vector<Item> items =
        m_monitor->getAllItems(filterGroups, filterTemplates, filterHosts);

    for(const Item& item : items) {
      std::unique_ptr<zabmon::Item> itemResult = std::make_unique<zabmon::Item>();
      GRPCAdapter::fromItem(item, itemResult.get());
      response->mutable_items()->AddAllocated(itemResult.release());
    }

  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix,");
  } catch(const std::exception& e) {
    std::cerr<<"Cant list Items" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't list Items, invalid arguments");
  }
  return ::grpc::Status::OK;
}

::grpc::Status Service::createGroup(
    ::grpc::ServerContext* /*context*/,
    const zabmon::CreateGroupRequest* request,
    zabmon::Group* response)
{
  HostGroup group;
  try {
    group = GRPCAdapter::toGroup(request->group());
    group.id = m_monitor->createGroup(group);
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix,");
  } catch(const std::exception& e) {
    std::cerr<<"Cant create Group" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't create Group, invalid arguments");
  }
  GRPCAdapter::fromGroup(group, response);
  return ::grpc::Status::OK;
}

::grpc::Status Service::createTemplate(
    ::grpc::ServerContext* /*context*/,
    const zabmon::CreateTemplateRequest* request,
    zabmon::Template* response)
{
  Template temp;
  try {
    temp = GRPCAdapter::toTemplate(request->template_());
    temp.id = m_monitor->createTemplate(temp);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant create Template" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't create Template, invalid arguments");
  }
  GRPCAdapter::fromTemplate(temp, response);
  return ::grpc::Status::OK;
}

::grpc::Status Service::createItem(
    ::grpc::ServerContext* /*context*/,
    const zabmon::CreateItemRequest* request,
    zabmon::Item* response)
{
  Item item;
  try {
    item = GRPCAdapter::toItem(request->item());
    item.id = m_monitor->createItem(item);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json," );
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant create Item" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't create Item, invalid arguments");
  }
  GRPCAdapter::fromItem(item, response);
  return ::grpc::Status::OK;
}

::grpc::Status Service::listItemsValues(
    ::grpc::ServerContext* /*context*/,
    const zabmon::ListItemsValuesRequest* request,
    zabmon::ListItemsValuesResponse* response)
{
  std::vector<ItemValues> values;
  try {
    std::vector<Item> items{};
    for(const zabmon::Item& item : request->items()) {
      items.push_back(GRPCAdapter::toItem(item));
    }
    int32_t limit = request->limit();
    values = m_monitor->getListValues(items, limit);
  } catch(const ConnectionError& e) {
    std::cerr<<"Invalid arguments for json," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid arguments for json,");
  } catch(const InvalidResponseError& e) {
    std::cerr<<"Invalid response from zabbix," <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Invalid response from zabbix," );
  } catch(const std::exception& e) {
    std::cerr<<"Cant list Item Values" <<e.what();
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                        "Can't list Item Values, invalid arguments");
  }
  GRPCAdapter::fromValues(values, response);
  return ::grpc::Status::OK;
}