#pragma once
#include "rtti.h"

struct FunctionArgument
{
    std::string m_data;
};

struct InvokeException
{
    std::string m_message;

    InvokeException(const std::string& message) :
        m_message(message)
    {
    }
};

template <typename FunctionType, typename ReturnType, typename... ArgumentTypes> struct Invoker;

template <typename FunctionType, typename ReturnType>
struct Invoker <FunctionType, ReturnType>
{
    template <typename... InvokeArguments>
    static ReturnType Invoke(const FunctionType& function, const std::vector<FunctionArgument>& functionArgument, size_t currentArgumentIndex, InvokeArguments... arguments)
    {
        return function(arguments...);
    }
};

template <typename FunctionType, typename ReturnType, typename Type, typename... ArgumentTypes>
struct Invoker <FunctionType, ReturnType, Type, ArgumentTypes...>
{
    template <typename... InvokeArguments>
    static ReturnType Invoke(const FunctionType& function, const std::vector<FunctionArgument>& functionArguments, size_t currentArgumentIndex, InvokeArguments... arguments)
    {
        assert(currentArgumentIndex < functionArguments.size());
        if (currentArgumentIndex >= functionArguments.size())
            throw InvokeException("Too few parameters");

        const auto currentArgument = Variable<Type>::Parse(functionArguments[currentArgumentIndex++]);
        return Invoker<FunctionType, ReturnType, ArgumentTypes...>::Invoke(function, functionArguments, currentArgumentIndex, arguments..., currentArgument);
    }
};