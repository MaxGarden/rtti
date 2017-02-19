#include <iostream>
#include "rtti.h"
#include "variable.h"
#include "function.h"
#include "object.h"

TestClass::TestClass()
{
    static auto id = 0;

    m_id = id++;
    std::cout << "CREATED INSTANCE (" << m_id << ")" << std::endl;
}

TestClass::~TestClass()
{
    std::cout << "DESTROYED INSTANCE (" << m_id << ")" << std::endl;
}

int main()
{
    RttiController controller;

    std::string command, name, type;

    while (true)
    {
        std::cin >> command >> name;

        if (command == "c")
        {
            std::cin >> type;
            std::cout << std::boolalpha << controller.CreateObject(name, type) << std::endl;
        }
        if (command == "d")
            std::cout << std::boolalpha << controller.DestroyObject(name) << std::endl;
    }

    VariableManager manager;
    manager.RegisterVariable<int>();

    FunctionBase* function = new Function<int&(int, float, const std::string& text)>([](int a, float b, const std::string& text)->int& {std::cout << "invoked!\n" << text << std::endl << a + b << std::endl; static int test = a; return test; });

    for (auto& arg : function->GetArgumentTypeNames())
        std::cout << arg << " ";

    std::cout << std::endl << std::endl;

    std::vector<FunctionArgument> arguments;
    arguments.resize(function->GetArgumentTypeNames().size());

    while (true)
    {
        for (auto i = 0u; i < arguments.size(); ++i)
        {
            std::cout << "(" << function->GetArgumentTypeNames()[i] << "): ";
            std::cin >> arguments[i].m_data;
        }

        try
        {
            if (const auto invoker = function->GetInvoker<int&>())
                std::cout << std::endl << "Return: " << invoker->Invoke(arguments) << std::endl;
            else
                std::cout << "Cannot get invoker" << std::endl;
        }
        catch (ParseException& error)
        {
            std::cout << error.m_message << std::endl;
        }
        catch (InvokeException& error)
        {
            std::cout << error.m_message << std::endl;
        }
    }
    std::cin.get();
    return 0;
}