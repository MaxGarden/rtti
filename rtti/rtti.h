#pragma once
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

using ArgumentsContainer = std::vector<std::string>;

template <typename Type>
std::string TypeName()
{
    return typeid(Type).name();
}

template <typename... Arguments> struct TypeNames;

template <>
struct TypeNames <>
{
    static void GetTypeName(ArgumentsContainer& buffer)
    {
    }
};

template <typename Type, typename... Arguments>
struct TypeNames <Type, Arguments...>
{
    static void GetTypeName(ArgumentsContainer& buffer)
    {
        buffer.push_back(TypeName<Type>());
        TypeNames<Arguments...>::GetTypeName(buffer);
    }
};
