#pragma once

#include <cstddef>

#define VTABLE_INDEX(offset) ((offset) / sizeof(std::uintptr_t))

namespace necromancy::memory {
template <typename Ret, typename Function, typename... Args>
Ret CallVTable(void* object, std::ptrdiff_t callIdx, Args... args)
{
    // ReSharper disable once CppReinterpretCastFromVoidPtr
    auto vtable = *reinterpret_cast<void***>(object);

    auto func = reinterpret_cast<Function>(vtable[callIdx]);

    return func(args...);
}
}
