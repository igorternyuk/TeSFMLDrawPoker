#pragma once

#include <string>
#include <sstream>
#include <iomanip>

template<class T>
std::string convertToStringWithPrecision(const T& val, int prec = 6)
{
    std::ostringstream oss;
    oss << std::setprecision(prec) << std::fixed << val;
    return oss.str();
}
