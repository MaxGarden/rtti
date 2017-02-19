#pragma once

using ArgumentsContainer = std::vector<std::string>;

template <typename Type>
struct ObjectType;

template <typename Type>
struct ObjectType <Type&> : ObjectType<Type> {};

template <typename Type>
struct ObjectType <const Type> : ObjectType<Type> {};

template <>
struct ObjectType <void>
{
    static const std::string& name;
};

#define REGISTER_RTTI_TYPE_IMPLEMENTATION(Type)                         \
    template <>                                                         \
    struct ObjectType<Type>                                             \
    {                                                                   \
    friend RttiController;                                              \
    private:                                                            \
        static Object CreateObject()                                    \
        {                                                               \
            return Object::Create<Type>();                              \
        }                                                               \
                                                                        \
    public:                                                             \
        static const std::string& name;                                 \
    };                                                                                                                                                   

#define REGISTER_RTTI_TYPE(Type)                                        \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type)                             \
    REGISTER_RTTI_TYPE_IMPLEMENTATION(Type*)                            \

class TypeInfo final
{
private:
    const std::string m_name;

    TypeInfo(const std::string& name) :
        m_name(name)
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
        return TypeInfo(ObjectType<Type>::name);
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
        buffer.push_back(ObjectType<Type>::name);
        TypeNames<Arguments...>::GetTypeName(buffer);
    }
};