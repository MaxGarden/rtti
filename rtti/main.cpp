#include <iostream>
#include "rtti.h"
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

    controller.CreateObject("one", "TestClass");
    controller.CreateObject("two", "TestClass");
    controller.CreateObject("three", "TestClass");

    IFunction* function = new Function<void(TestClass&, TestClass&, TestClass&)>([](TestClass& one, TestClass& two, TestClass& three) {std::cout << one.m_id << two.m_id << three.m_id; });

    std::vector<FunctionArgument> x = { { *controller.GetObject("one") }, { *controller.GetObject("two") }, { *controller.GetObject("three") } };

    function->Execute(x);

    controller.DestroyObject("two");
    
    std::cin.get();
    return 0;
}