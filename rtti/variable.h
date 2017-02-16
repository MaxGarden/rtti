#pragma once
#include "rtti.h"
#include "function.h"

class IVariable
{
public:
    virtual bool Parse(const std::string& data, void* buffer) = 0;
    virtual const std::string& GetTypeName() const = 0;
};

template <typename Type>
struct VariableBase : public IVariable
{
protected:
    std::string m_typeName;

public:

    VariableBase()
    {
        m_typeName = TypeName<Type>();
    }

    const std::string& GetTypeName() const override final
    {
        return m_typeName;
    }
};

template <typename Type>
struct Variable final : public VariableBase<Type>
{
public:
    virtual bool Parse(const std::string& data, void* buffer) override final
    {
        return false;
    }
};

template<>
struct Variable<int> final : public VariableBase<int>
{
    virtual bool Parse(const std::string& data, void* buffer) override final
    {
        return true;
    }

    static int Parse(const FunctionArgument& argument)
    {
        return atoi(argument.m_data.c_str());
    }
};

class VariableManager final
{
private:
    std::map<std::string, std::unique_ptr<IVariable>> m_variables;

public:
    IVariable* GetVariable(const std::string& name) const
    {
        const auto iterator = m_variables.find(name);

        if (iterator == m_variables.end())
            return nullptr;

        return iterator->second.get();
    }

    template <typename Type>
    IVariable* GetVariable()
    {
        return GetVariable(TypeName(Type));
    }

    template <typename Type>
    bool RegisterVariable()
    {
        const auto typeName = TypeName<Type>();

        const auto iterator = m_variables.find(typeName);
        if (iterator != m_variables.end())
            return false;

        m_variables.insert(std::make_pair(typeName, std::unique_ptr<IVariable>(new Variable<Type>())));
        return true;
    }

    template <typename Type>
    bool UnregisterVariable()
    {
        const auto typeName = TypeName<Type>();

        const auto iterator = m_variables.find(typeName);
        if (iterator == m_variables.end())
            return false;

        m_variables.erase(iterator);
        return true;
    }
};


