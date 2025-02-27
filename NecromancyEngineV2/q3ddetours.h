#pragma once

#include "hook.h"
#include "taggedexception.hpp"

namespace necromancy::hooks {
template <typename Function>
Hook<Function> AttachHook(Function target, Function detour)
{
    Hook<Function> hook(target, detour);
    auto status = hook.attach();
    if(status != Status::DetourException) // In this case can be a only 2 status codes - Ok or DetourException because AlreadyInUse and UnnecessaryOperation can be returned only after long-time using of hook object
    {
        return hook;
    }

    throw TaggedException<struct DetoursOperationCriticalTag>("Critical exception in Hooks lib");
}
}
