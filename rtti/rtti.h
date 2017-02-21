#pragma once
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <list>
#include "types.h"

class Object;

class RttiController final
{
private:
    void GatherRttiTypes();

protected:
    std::map<std::string, std::function<Object()>> m_spawningFunction;
    std::map<std::string, Object> m_objects;

public:
    RttiController();

    bool RegisterSpawnFunction(const std::string& name, std::function<Object()> SpawnFunction);
    bool UnregisterSpawnFunction(const std::string& name);

    bool CreateObject(const std::string& name, const std::string& typeName);
    bool CreateObject(const std::string& name, const TypeInfo& typeInfo);
    bool DestroyObject(const std::string& name);

    Object* GetObject(const std::string& name);
};