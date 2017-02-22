#pragma once
#include <algorithm>

template <typename Type>
struct ObjectType;

template <typename Type>
struct ObjectType <Type&> : ObjectType<Type> {};

template <typename Type>
struct ObjectType <const Type> : ObjectType<Type> {};

template <>
struct ObjectType <void>
{
    static const std::string& GetName()
    {
        static const std::string& name = "void";
        return name;
    }
    static const bool IsClass()
    {
        return false;
    }
};

#define REGISTER_RTTI_TYPE_IMPLEMENTATION(Type, isClass, ParentType)                                \
    template <>                                                                                     \
    struct ObjectType<Type>                                                                         \
    {                                                                                               \
    friend RttiController;                                                                          \
    private:                                                                                        \
        static Object CreateObject()                                                                \
        {                                                                                           \
            return Object::Create<Type>();                                                          \
        }                                                                                           \
                                                                                                    \
    public:                                                                                         \
        static const bool IsClass()                                                                 \
        {                                                                                           \
            return isClass;                                                                         \
        }                                                                                           \
                                                                                                    \
        static const std::string& GetName()                                                         \
        {                                                                                           \
            static const std::string name = []()                                                    \
            {                                                                                       \
                std::string result = #Type;                                                         \
                result.erase(std::remove_if(result.begin(), result.end(), isspace), result.end());  \
                return result;                                                                      \
            }();                                                                                    \
            return name;                                                                            \
        }                                                                                           \
                                                                                                    \
        static const ObjectType<ParentType> GetParentType()                                         \
        {                                                                                           \
            return ObjectType<ParentType>();                                                        \
        }                                                                                           \
    };                                                                                                                                                   

#define REGISTER_RTTI_TYPE(Type)                                                                    \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type, false, void)                                            \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type*, false, void)               

#define REGISTER_RTTI_TYPE_CLASS(Type, ParentType)                                                  \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type, true, ParentType)                                       \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type*, true, ParentType)          

class TypeInfo final
{
private:
    const std::string m_name;
    const bool m_isClass;

    TypeInfo(const std::string& name, const bool isClass) :
        m_name(name),
        m_isClass(isClass)
    {
    }

public:

    const std::string& GetName() const
    {
        return m_name;
    }

    const bool operator == (const TypeInfo& operand)
    {
        return m_name == operand.m_name;
    }

    template <typename Type>
    static TypeInfo Create()
    {
        return TypeInfo(ObjectType<Type>::GetName(), ObjectType<Type>::IsClass());
    }
};

template <typename... Arguments> struct TypeNames;

template <>
struct TypeNames <>
{
    static void GetTypeName(std::vector<TypeInfo>& buffer)
    {
    }
};

template <typename Type, typename... Arguments>
struct TypeNames <Type, Arguments...>
{
    static void GetTypeName(std::vector<TypeInfo>& buffer)
    {
        buffer.push_back(TypeInfo::Create<Type>());
        TypeNames<Arguments...>::GetTypeName(buffer);
    }
};