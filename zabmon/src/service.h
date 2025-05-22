#ifndef SERVICE_H
#define SERVICE_H

#include <monitor.h>

#include <atomic>
#include <memory>
#include <optional>
#include "zabmon.grpc.pb.h"

namespace zab::mon
{

/// @brief Класс, реализующий методы, сгенерированные из proto
class Service final : public zabmon::MonitorService::Service
{
 public:
  /// @brief Конструктор
  explicit Service(std::shared_ptr<Monitor> monitor);
  /// @brief Метод создания хоста
  ::grpc::Status createHost(
      ::grpc::ServerContext* context,
      const zabmon::CreateHostRequest* request,
      zabmon::Host* response) override;
  /// @brief Метод создания группы
  ::grpc::Status createGroup(
      ::grpc::ServerContext* context,
      const zabmon::CreateGroupRequest* request,
      zabmon::Group* response) override;
  /// @brief Метод создания шаблона
  ::grpc::Status createTemplate(
      ::grpc::ServerContext* context,
      const zabmon::CreateTemplateRequest* request,
      zabmon::Template* response) override;
  /// @brief Метод создания item
  ::grpc::Status createItem(
      ::grpc::ServerContext* context,
      const zabmon::CreateItemRequest* request,
      zabmon::Item* response) override;
  /// @brief Метод удаления хоста
  ::grpc::Status deleteHost(
      ::grpc::ServerContext* context,
      const zabmon::DeleteHostRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления группы
  ::grpc::Status deleteGroup(
      ::grpc::ServerContext* context,
      const zabmon::DeleteGroupRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления шаблона
  ::grpc::Status deleteTemplate(
      ::grpc::ServerContext* context,
      const zabmon::DeleteTemplateRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления итема
  ::grpc::Status deleteItem(
      ::grpc::ServerContext* context,
      const zabmon::DeleteItemRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод обновления хоста
  ::grpc::Status updateHost(
      ::grpc::ServerContext* context,
      const zabmon::UpdateHostRequest* request,
      zabmon::Host* response) override;
  /// @brief Метод получения списка всех хостов
  ::grpc::Status listHosts(
      ::grpc::ServerContext* context,
      const zabmon::ListHostsRequest* request,
      zabmon::ListHostsResponse* response) override;
  /// @brief Метод получения списка всех групп
  ::grpc::Status listGroups(
      ::grpc::ServerContext* context,
      const zabmon::ListGroupsRequest* request,
      zabmon::ListGroupsResponse* response) override;
  /// @brief Метод получения списка всех шаблонов
  ::grpc::Status listTemplates(
      ::grpc::ServerContext* context,
      const zabmon::ListTemplatesRequest* request,
      zabmon::ListTemplatesResponse* response) override;
  /// @brief Метод получения списка всех items
  ::grpc::Status listItems(
      ::grpc::ServerContext* context,
      const zabmon::ListItemsRequest* request,
      zabmon::ListItemsResponse* response) override;
  /// @brief Метод получения списка значений
  ::grpc::Status listItemsValues(
      ::grpc::ServerContext* context,
      const zabmon::ListItemsValuesRequest* request,
      zabmon::ListItemsValuesResponse* response) override;

 private:
  std::shared_ptr<Monitor> m_monitor;
};

}  // namespace zab::mon

#endif  // SERVICE_H
