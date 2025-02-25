#pragma once

#include <functional>
#include <stdexcept>

#include <detours.h>

#define STATIC_DYNAMIC_CALL \
    template<typename Ret, typename ...Args> \
    static Ret staticDynamicCall(void* object, const char* funcName, Args... args) { \
        return _instance->dynamicCall<Ret>(object, funcName, args...); \
    } \

namespace necromancy::hooks {

class ProxyMemoryObject {
    using AnonCFunc = void*;

public:
    template<typename Ret, typename ...Args>
    Ret dynamicCall(void* object, const char* funcName, Args... args);

    virtual ~ProxyMemoryObject() = default;

protected:
    explicit ProxyMemoryObject(const char* moduleName);

    /// @brief tries to find and add function with given name from module
    std::size_t add(const char* functionName);

    static bool allValid(ProxyMemoryObject* object);

    std::unordered_map<std::size_t, AnonCFunc> _functions;

    const char* _moduleName;
};

/// @brief Allows to call an arbitrary function inside of module that current object wraps
template<typename Ret, typename ...Args>
Ret ProxyMemoryObject::dynamicCall(void* object, const char* funcName, Args ...args) {
    void* function = DetourFindFunction(_moduleName, funcName);

    if(function == nullptr) {
        throw new std::runtime_error("Error detouring function");
    }

    return function(object, args...);
}

}
