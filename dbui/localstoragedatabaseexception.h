#pragma once

#include <exception>
#include <string>

struct LocalStorageDatabaseException : public std::exception
{

    LocalStorageDatabaseException(const std::string& what);

    virtual const char* what()  const throw();


    std::string what_;
};


