#ifndef GRPCADAPTER_H
#define GRPCADAPTER_H

#include <stcd-monitor/global.h>
#include <stcd-monitor/structures/host.h>
#include <stcd-monitor/structures/itemvalues.h>

#include <stcd-monitor-proto/messages.pb.h>

namespace zab::mon
{
/// @brief Класс для заполнения структур из gRPC
class GRPCAdapter
{
 private:

 public:
  /// @brief Метод конвертации хоста из gRPC в структуру
  /// @param host grpc структура хоста
  static Host toHost(const proto::Host& host);
  /// @brief Метод конвертации группы из gRPC в структуру
  /// @param group grpc структура группы
  static HostGroup toGroup(const proto::Group& group);
  /// @brief Метод конвертации шаблона из gRPC в структуру
  /// @param temp grpc структура шаблона
  static Template toTemplate(const proto::Template& temp);
  /// @brief Метод конвертации итема из gRPC в структуру
  /// @param item grpc структура итема
  static Item toItem(const proto::Item& item);
  /// @brief Метод конвертации хоста из структуры в gRPC
  /// @param host структура хоста
  /// @param result gRPC структура хоста
  static void fromHost(const Host& host, OUT proto::Host* result);
  /// @brief Метод конвертации структуры интерфейса в gRPC
  /// @param interfaces структура интерфейса
  /// @param result gRPC структура интерфейса
  static void fromInterface(const Interface& interface,
                            OUT proto::Interface* result);
  /// @brief Метод конвертации из группы в gRPC
  /// @param hostGroup группа
  /// @param result группа в формате gRPC
  static void fromGroup(const HostGroup& hostGroup, OUT proto::Group* result);
  /// @brief Метод конвертации из списка групп в gRPC
  /// @param allGroups список групп
  /// @param result список групп в формате gRPC
  static void fromGroups(const std::vector<HostGroup>& allGroups,
                         OUT proto::ListGroupsResponse* result);
  /// @brief Метод конвертации структуры шаблона в gRPC
  /// @param templates структура шаблона
  /// @param result gRPC структура шаблона
  static void fromTemplate(const Template& temp, OUT proto::Template* result);
  /// @brief Метод конвертации структуры элемента данных в gRPC
  /// @param items структура элементов данных
  /// @param result gRPC структура элемента данных
  static void fromItem(const Item& item, OUT proto::Item* result);
  /// @brief Метод конвертации из списка значений в gRPC
  /// @param itemsValues список значений
  /// @param result список значений в формате gRPC
  static void fromValues(const std::vector<ItemValues>& itemsValues,
                         OUT proto::ListItemsValuesResponse* result);
};
}  // namespace zab::mon

#endif  // GRPCADAPTER_H