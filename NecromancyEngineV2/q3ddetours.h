#pragma once

#include "hook.h"

namespace Necromancy {
namespace Detours {

template<typename Function>
Hook<Function> AttachHook(Function target, Function detour)
{
    Hook<Function> hook(target, detour);
    auto status = hook.attach();
    if(status != Status::DetourException) // In this case can be a only 2 status codes - Ok or DetourException because AlreadyInUse and UnnecessaryOperation can be returned only after long-time using of hook object
    {
        return hook;
    }

    throw std::runtime_error("Critical exception in Detours lib");
}

}
}

