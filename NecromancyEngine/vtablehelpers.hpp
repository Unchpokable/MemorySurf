#pragma once

template<typename Ret, typename Function, typename ...Args>
Ret CallVTable(void* object, ptrdiff_t callIdx, Args... args) {
    auto vtable = *reinterpret_cast<void***>(object);

    auto func = reinterpret_cast<Function>(vtable[callIdx]);

    return func(args...);
}