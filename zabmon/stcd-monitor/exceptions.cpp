#include <stcd-monitor/exceptions.h>

using namespace zab::mon;

MonitorException::MonitorException(std::string error)
    : m_error(std::move(error))
{
}

const char* MonitorException::what() const noexcept { return m_error.c_str(); }

ConnectionError::ConnectionError(std::string error)
    : MonitorException(error.empty() ? ("Can't connect to Zabbix")
                                     : std::move(error))

{
}

InvalidResponseError::InvalidResponseError(std::string error)
    : MonitorException(error.empty() ? ("Invalid type of response")
                                     : std::move(error))

{
}

ItemStatusError::ItemStatusError(std::string error)
    : MonitorException(error.empty() ? ("Item is disabled")
                                     : std::move(error))

{
}

ValueTypeError::ValueTypeError(std::string error)
    : MonitorException(error.empty() ? ("Value Type is invalid")
                                     : std::move(error))

{
}
