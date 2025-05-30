#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

namespace zab::mon

{
/// @brief Класс ошибок
class ZabMonException : public std::exception
{
 public:
  /// @brief конструктор
  /// @param error - ошибка, которая устанавливается в what()
  explicit ZabMonException(std::string error = "");
  [[nodiscard]] const char* what() const noexcept override;

 private:
  const std::string m_error;
};
/// @brief Ошибка, если не удается подключиться к заббиксу
class ConnectionError : public ZabMonException
{
 public:
  /// @brief Конструктор
  /// @param error - ошибка, которая устанавливается в what()
  explicit ConnectionError(std::string error = "");
};
/// @brief Ошибка, если ответ пришел не того типа
class InvalidResponseError : public ZabMonException
{
 public:
  /// @brief Конструктор
  /// @param error - ошибка, которая устанавливается в what()
  explicit InvalidResponseError(std::string error = "");
};
/// @brief Ошибка, если итем недоступен
class ItemStatusError : public ZabMonException
{
 public:
  /// @brief Конструктор
  /// @param error - ошибка, которая устанавливается в what()
  explicit ItemStatusError(std::string error = "");
};
/// @brief Ошибка, если тип значения итема не определён
class ValueTypeError : public ZabMonException
{
 public:
  /// @brief Конструктор
  /// @param error - ошибка, которая устанавливается в what()
  explicit ValueTypeError(std::string error = "");
};
}  // namespace zab::mon

#endif  // EXCEPTIONS_H
