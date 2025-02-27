#pragma once

#include <detours.h>
#include <functional>

#include "virtualfunction.h"

#define STATIC_DYNAMIC_CALL \
    template<typename Ret, typename ...Args> \
    static Ret staticDynamicCall(void* object, const char* funcName, Args... args) { \
        return _instance->dynamicCall<Ret>(object, funcName, args...); \
    } \
    \
    template<typename Ret, typename ...Args> \
    static VirtualFunction<Ret(__thiscall*)(Args...)> staticDynamicFunc(const char* funcName) { \
        return _instance->dynamicFunc<Ret, Args>(funcName); \
    }

namespace necromancy::hooks {
class ProxyMemoryObject
{
    using AnonCFunc = void*;

public:
    template <typename Ret, typename... Args>
    Ret dynamicCall(void* object, const char* funcName, Args... args);

    template <typename Ret, typename... Args>
    VirtualFunction<Ret(__thiscall*)(Args...)> dynamicFunc(const char* funcName);

    virtual ~ProxyMemoryObject() = default;

protected:
    explicit ProxyMemoryObject(const char* moduleName);

    /// @brief tries to find and add function with given name from module
    std::size_t add(const char* functionName);

    static bool allValid(ProxyMemoryObject* object);

    std::unordered_map<std::size_t, AnonCFunc> _functions;

    const char* _moduleName;
};

/// @brief Allows to call an arbitrary function by its name inside of module that current object wraps
template <typename Ret, typename... Args>
Ret ProxyMemoryObject::dynamicCall(void* object, const char* funcName, Args... args)
{
    using fnType = Ret(__thiscall*)(Args...);

    VirtualFunction<fnType> func = dynamicFunc<Ret, Args>(funcName);

    return func(object, args...);
}

/// @brief Allows to find an arbitrary function by its name inside of module that current object wraps
template <typename Ret, typename... Args>
VirtualFunction<Ret(__thiscall*)(Args...)> ProxyMemoryObject::dynamicFunc(const char* funcName)
{
    using fnType = Ret(__thiscall*)(Args...);

    fnType function = DetourFindFunction(_moduleName, funcName);

    return function;
}
}
