#include "localstoragedatabaseexception.h"

LocalStorageDatabaseException::LocalStorageDatabaseException(const std::string& what)
    : what_(what)
{

}

const char *     LocalStorageDatabaseException::what() const noexcept
{
    return what_.c_str();
}

