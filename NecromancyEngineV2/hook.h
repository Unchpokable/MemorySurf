#pragma once

namespace Necromancy::Detours {

enum class Status {
    AlreadyInUse, UnnecessaryOperation, DetourException, Ok, InvalidHookMode
};

class Hook final {
public:
    Hook(void* target, void* detour);
    Hook();
    ~Hook();

    Hook(const Hook& other) = delete;
    Hook(Hook&& other) = delete;
    Hook& operator=(const Hook& other) = delete;
    Hook& operator=(Hook&& other) = delete;

    Status attach();
    Status detach();

    template<typename Function>
    Function original() const noexcept;

    template<typename Function>
    Function detour() const noexcept;

private:
    void* _targetFunction;
    void* _detourFunction;
    bool _hookAttached;
};

template<typename Function>
Function Hook::original() const noexcept {
    return reinterpret_cast<Function>(_targetFunction);
}

template<typename Function>
Function Hook::detour() const noexcept {
    return reinterpret_cast<Function>(_detourFunction);
}

} // namespace Necromancy::Detours