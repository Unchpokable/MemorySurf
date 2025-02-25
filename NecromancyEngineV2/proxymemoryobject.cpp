#include "pch.h"

#include "proxymemoryobject.h"

using namespace necromancy::hooks;

ProxyMemoryObject::ProxyMemoryObject(const char *moduleName) : _moduleName(moduleName) {
    if(moduleName == nullptr) {
        throw std::runtime_error("must provide a valid module name when creating a ProxyMemoryObject");
    }
}

std::size_t ProxyMemoryObject::add(const char* functionName) {
    auto function = DetourFindFunction(_moduleName, functionName);

    if(function == nullptr) {
        throw std::runtime_error("Given function does not exports by given module");
    }

    auto id = std::hash<std::string>{}(functionName);

    _functions.insert_or_assign(id, function);

    return id;
}
