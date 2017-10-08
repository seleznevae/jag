#ifndef ENUM_H
#define ENUM_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include <cctype>


namespace string_utility {
inline std::vector<std::string> splitString(const std::string& str, char separator = ',')
{
    auto stripString = [](const std::string& str) -> std::string {
        auto it_beg = std::find_if(str.begin(), str.end(), [](char c) { return std::isalnum(c); });
        auto it_end = std::find_if(it_beg, str.end(), [](char c) { return !std::isalnum(c); });
        return std::string(it_beg, it_end);
    };
    std::vector<std::string> ret;
    auto it = std::begin(str);
    while (it != std::end(str))
    {
        auto beg = it;
        it = std::find(beg, std::end(str), separator);
        std::string new_el(beg, it);
        ret.push_back(stripString(new_el));
        if (it != std::end(str))
            ++it;
    }
    return ret;
}
}

template <typename T>
std::vector<std::string> getEnumNames()
{
    static_assert(sizeof(T) == 0, "getEnumNames is not specialized for this enum\n");
    return std::vector<std::string>();
}
template <typename T>
std::vector<T> getEnumValuesList();


template <typename T>
std::string enumToString(T arg)
{
    return getEnumNames<T>()[static_cast<int>(arg)];
}

template <typename T>
T stringToEnum(const std::string& arg, bool *is_ok = nullptr)
{
    auto stateNames = getEnumNames<T>();
    auto it = std::find(std::begin(stateNames), std::end(stateNames), arg);
    if (it == std::end(stateNames)) {
        if (is_ok == nullptr) {
            throw std::bad_cast();
        } else {
            *is_ok = false;
            return static_cast<T>(0);
        }
    }
    if (is_ok)
        *is_ok = true;
    return static_cast<T>(it - std::begin(stateNames));
}

template <typename T, typename U, typename Dummy = typename std::enable_if<!std::is_enum<U>::value>::type>
T lexical_cast(const U& arg, typename std::enable_if<!std::is_enum<T>::value, T>::type * = nullptr)
{
    T retValue;
   std::stringstream stream;
   if (!(stream << arg) || !(stream >> retValue)
       || ! (stream >> std::ws).eof())
       throw std::bad_cast();
    return retValue;
}

template <typename T, typename U>
typename std::enable_if<std::is_enum<T>::value, T>::type lexical_cast(const U& arg)
{
   std::string stringRepresent;
   std::stringstream stream;
   if (!(stream << arg) || !(stream >> stringRepresent)
       || ! (stream >> std::ws).eof())
       throw std::bad_cast();
    return stringToEnum<T>(stringRepresent);
}

template <typename U, typename T,
        typename Dummy = typename std::enable_if<std::is_enum<T>::value>::type,
        typename Dummy2 = typename std::enable_if<std::is_same<U, std::string>::value>::type
            >
U lexical_cast(T arg)
{
    return enumToString(arg);
}


#define DEFINE_ENUM(Enum_name, Enum_type, ...) \
enum class Enum_name: Enum_type { __VA_ARGS__ }; \
template<> \
std::vector<std::string> getEnumNames<Enum_name>() { \
    static std::vector<std::string> ret = string_utility::splitString( #__VA_ARGS__);    \
    return ret; \
}\
template<>\
std::vector<Enum_name> getEnumValuesList<Enum_name>() { \
    return ret{__VA_ARGS__}; \
}




#endif // ENUM_H
