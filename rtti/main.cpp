#include <iostream>
#include "rtti.h"
#include "variable.h"
#include "function.h"

int main()
{
    VariableManager manager;
    manager.RegisterVariable<int>();

    static const std::function<int(int, int)> func = [](int a, int b)->int {std::cout << "invoked!\n" << a + b << std::endl; return a + b; };

    IFunction* function = new Function<int, int, int>(func);

    for (auto& arg : function->GetArguments())
        std::cout << arg << std::endl;

    std::vector<FunctionArgument> args;
    args.push_back({ "100" });
    args.push_back({ "4504" });

    function->Execute(args);

    std::cin.get();
    return 0;
}