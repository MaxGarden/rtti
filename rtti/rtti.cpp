#include "rtti.h"
#include "object.h"

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
        RegisterSpawnFunction(ObjectType<Type>::GetName(), ObjectType<Type>::CreateObject);  \
        RegisterSpawnFunction(ObjectType<Type*>::GetName(), ObjectType<Type>::CreateObject);
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

Object* RttiController::GetObject(const std::string& name)
{
    const auto iterator = m_objects.find(name);

    return iterator == m_objects.end() ? nullptr : &iterator->second;
}