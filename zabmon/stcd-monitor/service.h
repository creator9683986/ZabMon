#ifndef SERVICE_H
#define SERVICE_H

#include <stcd-monitor/monitor.h>

#include <atomic>
#include <memory>
#include <optional>

#include <stcd-monitor-proto/services.grpc.pb.h>

namespace zab::mon
{

/// @brief Класс, реализующий методы, сгенерированные из proto
class Service final : public proto::MonitorService::Service
{
 public:
  /// @brief Конструктор
  explicit Service(std::shared_ptr<Monitor> monitor);
  /// @brief Метод создания хоста
  ::grpc::Status createHost(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::CreateHostRequest* request,
      ::zab::mon::proto::Host* response) override;
  /// @brief Метод создания группы
  ::grpc::Status createGroup(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::CreateGroupRequest* request,
      ::zab::mon::proto::Group* response) override;
  /// @brief Метод создания шаблона
  ::grpc::Status createTemplate(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::CreateTemplateRequest* request,
      ::zab::mon::proto::Template* response) override;
  /// @brief Метод создания item
  ::grpc::Status createItem(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::CreateItemRequest* request,
      ::zab::mon::proto::Item* response) override;
  /// @brief Метод удаления хоста
  ::grpc::Status deleteHost(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::DeleteHostRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления группы
  ::grpc::Status deleteGroup(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::DeleteGroupRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления шаблона
  ::grpc::Status deleteTemplate(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::DeleteTemplateRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод удаления итема
  ::grpc::Status deleteItem(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::DeleteItemRequest* request,
      ::google::protobuf::Empty* response) override;
  /// @brief Метод обновления хоста
  ::grpc::Status updateHost(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::UpdateHostRequest* request,
      ::zab::mon::proto::Host* response) override;
  /// @brief Метод получения списка всех хостов
  ::grpc::Status listHosts(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::ListHostsRequest* request,
      ::zab::mon::proto::ListHostsResponse* response) override;
  /// @brief Метод получения списка всех групп
  ::grpc::Status listGroups(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::ListGroupsRequest* request,
      ::zab::mon::proto::ListGroupsResponse* response) override;
  /// @brief Метод получения списка всех шаблонов
  ::grpc::Status listTemplates(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::ListTemplatesRequest* request,
      ::zab::mon::proto::ListTemplatesResponse* response) override;
  /// @brief Метод получения списка всех items
  ::grpc::Status listItems(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::ListItemsRequest* request,
      ::zab::mon::proto::ListItemsResponse* response) override;
  /// @brief Метод получения списка значений
  ::grpc::Status listItemsValues(
      ::grpc::ServerContext* context,
      const ::zab::mon::proto::ListItemsValuesRequest* request,
      ::zab::mon::proto::ListItemsValuesResponse* response) override;

 private:
  std::shared_ptr<Monitor> m_monitor;
};

}  // namespace zab::mon

#endif  // SERVICE_H
