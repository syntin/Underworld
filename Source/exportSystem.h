//DB
#pragma once
#include <string>
#include <vector>
#include <typeinfo>

struct ExportedVariable
{
	std::string name;
	std::string typeName;
	void* ownerPtr;
	void* valuePtr;
	size_t size;
};

class ExportRegistry
{
public:
    static ExportRegistry& Instance()
    {
        static ExportRegistry inst;
        return inst;
    }

    void Register(const ExportedVariable& var)
    {
        exportedVars.push_back(var);
    }

    const std::vector<ExportedVariable>& GetAll() const
    {
        return exportedVars;
    }

private:
    std::vector<ExportedVariable> exportedVars;
};



#define EXPORT(owner, variable) \
    ExportRegistry::Instance().Register({ \
        #variable, \
        typeid(decltype((owner).variable)).name(), \
        (void*)&(owner), \
        (void*)&((owner).variable), \
        sizeof((owner).variable) \
    })
