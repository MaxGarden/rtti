#include "rtti.h"
#include "object.h"

const std::string& ObjectType<void>::name = "void";
#pragma push_macro("REGISTER_RTTI_TYPE")
#undef REGISTER_RTTI_TYPE
#define REGISTER_RTTI_TYPE(Type)                        \
    const std::string& ObjectType<Type>::name = #Type;  \
    const std::string& ObjectType<Type*>::name = #Type;
#include "types.inl"
#pragma pop_macro("REGISTER_RTTI_TYPE")

RttiController::RttiController()
{
    GatherRttiTypes();
}

void RttiController::GatherRttiTypes()
{
    #pragma push_macro("REGISTER_RTTI_TYPE")
    #undef REGISTER_RTTI_TYPE
    #define REGISTER_RTTI_TYPE(Type)                                                    \
        RegisterSpawnFunction(ObjectType<Type>::name, ObjectType<Type>::CreateObject);  \
        RegisterSpawnFunction(ObjectType<Type*>::name, ObjectType<Type>::CreateObject);
    #include "types.inl"
    #pragma pop_macro("REGISTER_RTTI_TYPE")
}

bool RttiController::RegisterSpawnFunction(const std::string& name, std::function<Object()> spawnFunction)
{
    const auto iterator = m_spawningFunction.find(name);

    if (iterator != m_spawningFunction.end())
        return false;

    m_spawningFunction.insert(std::make_pair(name, spawnFunction));

    return true;
}

bool RttiController::UnregisterSpawnFunction(const std::string& name)
{
    const auto iterator = m_spawningFunction.find(name);

    if (iterator == m_spawningFunction.end())
        return false;

    m_spawningFunction.erase(iterator);

    return true;
}