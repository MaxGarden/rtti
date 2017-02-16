#pragma once
#include "rtti.h"
#include "invoker.h"

template<typename ReturnType>
struct FunctionInvoker;

struct FunctionBase
{
    virtual const ArgumentsContainer& GetArguments() const
    {
        static ArgumentsContainer nullValue;
        return nullValue;
    }

    virtual const type_info& GetReturnTypeInfo() const
    {
        return typeid(void);
    }

    template <typename ReturnType>
    FunctionInvoker<ReturnType>* GetInvoker()
    {
        return dynamic_cast<FunctionInvoker<ReturnType>*>(this);
    }
};

template <typename ReturnType>
struct FunctionInvoker : public FunctionBase
{
public:
    virtual ReturnType Invoke(const std::vector<FunctionArgument>& arguments) = 0;

    virtual const type_info& GetReturnTypeInfo() const override final
    {
        return typeid(ReturnType);
    }
};

template <typename ReturnType, typename... ArgumentTypes>
struct Function : public FunctionInvoker<ReturnType>
{
    using FunctionType = std::function<ReturnType(ArgumentTypes...)>;

private:
    const FunctionType m_function;
    ArgumentsContainer m_argumentsNames;

public:
    Function(const FunctionType& function) :
        m_function(function)
    {
        TypeNames<ArgumentTypes...>::GetTypeName(m_argumentsNames);
    }

    virtual ReturnType Invoke(const std::vector<FunctionArgument>& arguments) override final
    {
        return Invoker<FunctionType, ReturnType, ArgumentTypes...>::Invoke(m_function, arguments, 0);
    }

    const ArgumentsContainer& GetArguments() const override final
    {
        return m_argumentsNames;
    }
};

template <typename ReturnType, typename... ArgumentTypes>
struct Function <std::function<ReturnType(ArgumentTypes...)>> final : public Function <ReturnType, ArgumentTypes...>
{
    Function(const FunctionType& function) : Function < ReturnType, ArgumentTypes...>(function) {};
};

template <typename ReturnType, typename... ArgumentTypes>
struct Function <ReturnType(ArgumentTypes...)> final : public Function <ReturnType, ArgumentTypes...>
{
    Function(const FunctionType& function) : Function < ReturnType, ArgumentTypes...>(function) {};
};


