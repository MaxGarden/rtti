#include <iostream>
#include "rtti.h"
#include "variable.h"
#include "function.h"

int main()
{
    VariableManager manager;
    manager.RegisterVariable<int>();

    FunctionBase* function = new Function<int&(int, float)>([](int a, float b)->int& {std::cout << "invoked!\n" << a + b << std::endl; static int test = 111; return test; });

    for (auto& arg : function->GetArguments())
        std::cout << arg << std::endl;

    if (const auto invoker = function->GetInvoker<int&>())
    {
        auto returnValue = invoker->Invoke({ { "10" }, { "20" } });

        std::cout << std::endl << "Return: " << returnValue;
    }
    else
        std::cout << "Cannot get invoker";

    std::cin.get();
    return 0;
}