#pragma once
#include "invoker.h"

class IFunction
{
public:
    virtual ~IFunction() {};
    virtual Object Execute(std::vector<FunctionArgument>& arguments) = 0;
    virtual const TypeInfo& GetReturnTypeInfo() const = 0;
    virtual const std::vector<TypeInfo>& GetArgumentTypeInfo() const = 0;
};

template <typename ReturnType, typename... ArgumentTypes>
class Function : public IFunction
{
public:
    using FunctionType = std::function<ReturnType(ArgumentTypes...)>;

private:
    template <typename Type = ReturnType>
    Object ExecuteImplementation(std::vector<FunctionArgument>& arguments, typename std::enable_if<!std::is_same<typename Type, void>::value>::type* = 0)
    {
        return Invoker<FunctionType, ReturnType, ArgumentTypes...>::Invoke(m_function, arguments, 0);
    }

    template <typename Type = ReturnType>
    Object ExecuteImplementation(std::vector<FunctionArgument>& arguments, typename std::enable_if<std::is_same<Type, void>::value>::type* = 0)
    {
        Invoker<FunctionType, ReturnType, ArgumentTypes...>::Invoke(m_function, arguments, 0);
        return Object();
    }

private:
    const FunctionType m_function;
    TypeInfo m_returnTypeInfo;
    std::vector<TypeInfo> m_argumentTypeInfo;

public:
    Function(const FunctionType& function) :
        m_function(function),
        m_returnTypeInfo(TypeInfo::Create<ReturnType>())
    {
        TypeNames<ArgumentTypes...>::GetTypeName(m_argumentTypeInfo);
    }

    virtual Object Execute(std::vector<FunctionArgument>& arguments) override final
    {
        return ExecuteImplementation(arguments);
    }

    const TypeInfo& GetReturnTypeInfo() const override final
    {
        return m_returnTypeInfo;
    }

    const std::vector<TypeInfo>& GetArgumentTypeInfo() const override final
    {
        return m_argumentTypeInfo;
    }
};

template <typename ReturnType, typename... ArgumentTypes>
class Function <std::function<ReturnType(ArgumentTypes...)>> final : public Function <ReturnType, ArgumentTypes...>
{
public:
    Function(const FunctionType& function) : Function < ReturnType, ArgumentTypes...>(function) {};
};

template <typename ReturnType, typename... ArgumentTypes>
class Function <ReturnType(ArgumentTypes...)> final : public Function <ReturnType, ArgumentTypes...>
{
public:
    Function(const FunctionType& function) : Function < ReturnType, ArgumentTypes...>(function) {};
};