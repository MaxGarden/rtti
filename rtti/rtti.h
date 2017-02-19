#pragma once
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <experimental\string>
#include "types.h"

class Object;

class RttiController final
{
private:
    void GatherRttiTypes();

protected:
    std::map<std::string, std::function<Object()>> m_spawningFunction;

public:
    RttiController();

    bool RegisterSpawnFunction(const std::string& name, std::function<Object()> SpawnFunction);
    bool UnregisterSpawnFunction(const std::string& name);
};