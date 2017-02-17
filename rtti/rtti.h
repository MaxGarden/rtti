#pragma once
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

using ArgumentsContainer = std::vector<std::string>;

struct TypeInfo
{
    const type_info& m_typeInfo;

    TypeInfo() :
        m_typeInfo(typeid(void))
    {
    }

    TypeInfo(const type_info& typeInfo) :
        m_typeInfo(typeInfo)
    {
    }

    const std::string& GetName() const
    {
        static std::string staticName = m_typeInfo.name();
        return staticName;
    }

    template <typename Type>
    static TypeInfo Create()
    {
        return TypeInfo(typeid(Type));
    }

    const bool operator == (const TypeInfo& operand)
    {
        return m_typeInfo == operand.m_typeInfo;
    }
};

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
        buffer.push_back(TypeInfo::Create<Type>().GetName());
        TypeNames<Arguments...>::GetTypeName(buffer);
    }
};
