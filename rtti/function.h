#pragma once
#include "rtti.h"
#include "invoker.h"

class IFunction
{
public:

    virtual void* Execute(const std::vector<FunctionArgument>& arguments) = 0;
    virtual const ArgumentsContainer& GetArguments() const = 0;
};


template <typename ReturnType, typename... Arguments>
class Function final : public IFunction
{
    using FunctionType = std::function<ReturnType(Arguments...)>;

    void Caller(Arguments... arguments)
    {
        m_function(arguments...);
    }

private:
    std::unique_ptr<ReturnType> m_lastReturnValue;
    const FunctionType& m_function;
    ArgumentsContainer m_argumentsNames;

public:
    Function(const FunctionType& function) :
        m_function(function)
    {
        TypeNames<Arguments...>::GetTypeName(m_argumentsNames);
    }


    virtual void* Execute(const std::vector<FunctionArgument>& arguments) override final
    {
        Invoker<FunctionType, Arguments...>::Invoke(m_function, arguments, 0);
        return GetLastReturnValue();
    }

    ReturnType* GetLastReturnValue() const
    {
        return m_lastReturnValue.get();
    }

    const ArgumentsContainer& GetArguments() const override final
    {
        return m_argumentsNames;
    }
};
