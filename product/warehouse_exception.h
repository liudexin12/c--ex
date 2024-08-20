#ifndef WAREHOUSE_EXCEPTION_H
#define WAREHOUSE_EXCEPTION_H

#include <exception>
#include <string>
#include "log/logg.h"

class WarehouseException : public std::exception
{
public:
    explicit WarehouseException(const std::string &message) : message_(message) {}

    virtual const char *what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif
