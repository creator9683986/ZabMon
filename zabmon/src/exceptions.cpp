#include <exceptions.h>

using namespace zab::mon;

ZabMonException::ZabMonException(std::string error)
    : m_error(std::move(error))
{
}

const char* ZabMonException::what() const noexcept { return m_error.c_str(); }

ConnectionError::ConnectionError(std::string error)
    : ZabMonException(error.empty() ? ("Can't connect to Zabbix")
                                     : std::move(error))

{
}

InvalidResponseError::InvalidResponseError(std::string error)
    : ZabMonException(error.empty() ? ("Invalid type of response")
                                     : std::move(error))

{
}

ItemStatusError::ItemStatusError(std::string error)
    : ZabMonException(error.empty() ? ("Item is disabled")
                                     : std::move(error))

{
}

ValueTypeError::ValueTypeError(std::string error)
    : ZabMonException(error.empty() ? ("Value Type is invalid")
                                     : std::move(error))

{
}
