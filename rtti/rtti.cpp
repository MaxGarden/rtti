#include "rtti.h"
#include "object.h"
#include <algorithm>

static std::string AdjustObjectTypeName(std::string name)
{
    name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
    return name;
}

const std::string& ObjectType<void>::name = "void";
#pragma push_macro("REGISTER_RTTI_TYPE")
#undef REGISTER_RTTI_TYPE
#define REGISTER_RTTI_TYPE(Type)                                                \
    const std::string& ObjectType<Type>::name = AdjustObjectTypeName(#Type);    \
    const std::string& ObjectType<Type*>::name = AdjustObjectTypeName(#Type"*");
#include "types.inl"
#pragma pop_macro("REGISTER_RTTI_TYPE")

RttiController::RttiController()
{
    GatherRttiTypes();
}

void RttiController::GatherRttiTypes()
{
    #define RTTI_GATHERING_TYPES
    #pragma push_macro("REGISTER_RTTI_TYPE")
    #undef REGISTER_RTTI_TYPE
    #define REGISTER_RTTI_TYPE(Type)                                                    \
        RegisterSpawnFunction(ObjectType<Type>::name, ObjectType<Type>::CreateObject);  \
        RegisterSpawnFunction(ObjectType<Type*>::name, ObjectType<Type>::CreateObject);
    #include "types.inl"
    #pragma pop_macro("REGISTER_RTTI_TYPE")
    #undef RTTI_GATHERING_TYPES
}

bool RttiController::RegisterSpawnFunction(const std::string& name, std::function<Object()> spawnFunction)
{
    const auto iterator = m_spawningFunction.find(name);

    if (iterator != m_spawningFunction.end())
        return false;

    m_spawningFunction.insert(std::make_pair(name, spawnFunction));

    printf("Registered %s\n", name.c_str());

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

bool RttiController::CreateObject(const std::string& name, const std::string& typeName)
{
    if (m_objects.find(name) != m_objects.end())
        return false;

    const auto iterator = m_spawningFunction.find(typeName);

    if (iterator == m_spawningFunction.end())
        return false;

    m_objects.insert(std::make_pair(name, iterator->second()));

    return true;
}

bool RttiController::CreateObject(const std::string& name, const TypeInfo& typeInfo)
{
    return CreateObject(name, typeInfo.GetName());
}

bool RttiController::DestroyObject(const std::string& name)
{
    const auto iterator = m_objects.find(name);

    if (iterator == m_objects.end())
        return false;

    m_objects.erase(iterator);

    return true;
}