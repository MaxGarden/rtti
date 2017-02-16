#pragma once
#include "rtti.h"

struct FunctionArgument
{
    std::string m_data;
};

template <typename FunctionType, typename... Arguments> struct Invoker;

template <typename FunctionType>
struct Invoker <FunctionType>
{
    template <typename... InvokeArguments>
    static void Invoke(const FunctionType& function, const std::vector<FunctionArgument>& functionArgument, size_t currentArgumentIndex, InvokeArguments... arguments)
    {
        function(arguments...);
    }
};

template <typename FunctionType, typename Type, typename... Arguments>
struct Invoker <FunctionType, Type, Arguments...>
{
    template <typename... InvokeArguments>
    static void Invoke(const FunctionType& function, const std::vector<FunctionArgument>& functionArguments, size_t currentArgumentIndex, InvokeArguments... arguments)
    {
        assert(currentArgumentIndex < functionArguments.size());
        if (currentArgumentIndex >= functionArguments.size())
            return;

        const auto currentArgument = Variable<Type>::Parse(functionArguments[currentArgumentIndex++]);
        Invoker<FunctionType, Arguments...>::Invoke(function, functionArguments, currentArgumentIndex, currentArgument, arguments...);
    }
};
