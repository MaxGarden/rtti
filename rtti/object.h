#pragma once
#include "rtti.h"

struct CastException
{
    std::string m_message;

    CastException(const std::string& message) :
        m_message(message)
    {
    }
};

class Object
{
    struct PlaceHolder
    {
        virtual ~PlaceHolder() {};

        virtual const TypeInfo& GetTypeInfo() const = 0;
        virtual PlaceHolder* Copy() const = 0;
    };

    template <typename Type>
    struct Holder final : public PlaceHolder
    {
    protected:
        TypeInfo m_typeInfo;

    public:
        Type m_value;

        Holder() :
            m_typeInfo(TypeInfo::Create<Type>())
        {
        }

        Holder(const Type& value) :
            m_value(value),
            m_typeInfo(TypeInfo::Create<Type>())
        {
        }

        Holder(Type&& value) :
            m_value(static_cast<Type&&>(value)),
            m_typeInfo(TypeInfo::Create<Type>())
        {
        }

        virtual const TypeInfo& GetTypeInfo() const override final
        {
            return m_typeInfo;
        }

        virtual PlaceHolder* Copy() const override final
        {
            return new Holder<Type>(m_value);
        }
    };

protected:
    std::unique_ptr<PlaceHolder> m_object;

public:

    Object()
    {
    }

    template <typename Type>
    Object(const Type& value)
    {
        m_object.reset(new Holder<std::remove_cv<std::decay<const Type>::type>::type>(value));
    }

    Object(const Object& object)
    {
        m_object.reset(object.m_object ? object.m_object->Copy() : 0);
    }

    template <typename Type>
    Object(const Type&& value, typename std::enable_if<!std::is_same<Object&, Type>::value>::type* = nullptr, typename std::enable_if<std::is_const<Type>::value>::type* = nullptr)
    {
        m_object.reset(new Holder<typename std::decay<Type>::type>(static_cast<Type&&>(value)))
    }

    Object(Object&& object)
    {
        m_object = std::move(object.m_object);
    }

    const TypeInfo& GetTypeInfo() const
    {
        static auto nullValue = TypeInfo::Create<void>();
        return m_object ? m_object->GetTypeInfo() : nullValue;
    }

    template <typename Type>
    inline bool CanCast() const
    {
        return TypeInfo::Create<Type>() == GetTypeInfo();
    }

    template <typename Type>
    inline Type* Pointer() const
    {
        return CanCast<Type>() ? &static_cast<Holder<std::remove_cv<Type>::type>*>(m_object.get())->m_value : nullptr;
    }
    
    template <typename Type>
    Type Cast() const
    {
        if (auto result = Pointer<std::remove_reference<Type>::type>())
        {
            if (std::is_reference<Type>::value)
                return static_cast<Type>(*result);
            else
                return static_cast<std::add_lvalue_reference<Type>::type>(*result);
        }
        else
            throw CastException("Incompatible types");
    }

private:
template <typename Type>
friend struct ObjectType;

    template <typename Type>
    Object(Type*)
    {
        m_object.reset(new Holder<Type>());
    }

    template <typename Type>
    static Object Create()
    {
        return Object((Type*)nullptr);
    }
};

#include "types.inl"