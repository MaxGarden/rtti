#pragma once
#include "rtti.h"
#include "function.h"
#include <regex>

struct ParseException
{
    std::string m_message;

    ParseException(const std::string& typeName) :
        m_message("(" + typeName + ") cannot parse specified input")
    {
    }
};

class IVariable
{
public:
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
        m_typeName = TypeInfo::Create<Type>().GetName();
    }

    static Type Parse(const FunctionArgument& argument)
    {
        assert(false);
        throw ParseException(TypeInfo::Create<Type>().GetName());
    }

    const std::string& GetTypeName() const override final
    {
        return m_typeName;
    }
};

template <typename Type>
struct Variable : public VariableBase<Type>
{
};

template<>
struct Variable<signed int> : public VariableBase<int>
{
    static signed int Parse(const FunctionArgument& argument)
    {
        static std::regex regex("(\\+|-)?[0-9]+");

        if (!std::regex_match(argument.m_data, regex))
            throw ParseException("signed int");

        return atoi(argument.m_data.c_str());
    }
};

template<>
struct Variable<unsigned int> : public VariableBase<int>
{
    static unsigned int Parse(const FunctionArgument& argument)
    {
        static std::regex regex("\\+?[0-9]+");

        if (!std::regex_match(argument.m_data, regex))
            throw ParseException("unsigned int");

        return atoi(argument.m_data.c_str());
    }
};

template<>
struct Variable<float> : public VariableBase<float>
{
    static float Parse(const FunctionArgument& argument)
    {
        static std::regex regex("(\\+|-)?[0-9]+(\\.[0-9]+f?|\\.f?)?");

        if (!std::regex_match(argument.m_data, regex))
            throw ParseException("float");

        return (float)atof(argument.m_data.c_str());
    }
};

template<>
struct Variable<double> : public VariableBase<float>
{
    static double Parse(const FunctionArgument& argument)
    {
        static std::regex regex("(\\+|-)?[0-9]+(\\.[0-9]+|\\.)?");

        if (!std::regex_match(argument.m_data, regex))
            throw ParseException("double");

        return atof(argument.m_data.c_str());
    }
};

template<>
struct Variable<std::string> : public VariableBase<float>
{
    static std::string Parse(const FunctionArgument& argument)
    {
        return argument.m_data;
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
        const auto typeName = TypeInfo::Create<Type>().GetName();

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


