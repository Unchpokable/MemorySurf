#pragma once


namespace Necromancy::Detours {

enum class Status {
    AlreadyInUse, UnnecessaryOperation, DetourException, Ok, InvalidHookMode
};

template<typename Tag>
struct UnstableArg;

using Unstable = UnstableArg<struct Ret>(*)(UnstableArg<struct InOut>);

template<typename Function>
class Hook final
{
public:

    Hook(Function target, Function detour);
    Hook();

    ~Hook();

    Status attach();

    template<typename Func1, typename Func2>
    Status unstableAttach(Func1 target, Func2 detour);

    Status detach();
    Status unstableDetach();

    Function original() const noexcept;
    Function detour() const noexcept;

    std::any unstableOriginal() const noexcept;
    std::any unstableDetour() const noexcept;

private:
    Function _targetFunction;
    Function _detourFunction;

    std::any _unstableTarget;
    std::any _unstableDetour;

    bool _hookAttached;
};

template<typename Function>
Hook<Function>::Hook(Function target, Function detour) : _targetFunction(target), _detourFunction(detour), _hookAttached(false) {
    // empty
}

template<typename Function>
Hook<Function>::Hook() : _targetFunction(nullptr), _detourFunction(nullptr), _hookAttached(false) {
    // empty
}

template<typename Function>
Hook<Function>::~Hook() {
    detach();
}

template<typename Function>
Status Hook<Function>::attach() {
    if(_hookAttached)
        return Status::AlreadyInUse;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if(DetourAttach(&reinterpret_cast<PVOID&>(_targetFunction), _detourFunction) != NO_ERROR)
    {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _hookAttached = true;
    return Status::Ok;
}

template<typename Function>
template<typename Func1, typename Func2>
Status Hook<Function>::unstableAttach(Func1 target, Func2 detour) {
    if constexpr(!std::is_same_v<Function, Unstable>)
        return Status::InvalidHookMode;

    if(_hookAttached)
        return Status::AlreadyInUse;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if(DetourAttach(&reinterpret_cast<PVOID&>(target), detour) != NO_ERROR) {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _unstableTarget = target;
    _unstableDetour = detour;
    _hookAttached = true;
    return Status::Ok;
}

template<typename Function>
Status Hook<Function>::detach() {
    if(!_hookAttached)
        return Status::UnnecessaryOperation;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    if(DetourDetach(&reinterpret_cast<PVOID&>(_targetFunction), _detourFunction) != NO_ERROR)
    {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _hookAttached = false;
    return Status::Ok;
}

template<typename Function>
Status Hook<Function>::unstableDetach() {
    if constexpr (!std::is_same_v<Function, Unstable>)
    {
        return Status::UnnecessaryOperation;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    void* target = std::any_cast<PVOID>(_unstableTarget);

    if(DetourDetach(&reinterpret_cast<PVOID&>(target), std::any_cast<PVOID>(_unstableDetour)) != NO_ERROR)
    {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _hookAttached = false;
    return Status::Ok;
}

template<typename Function>
Function Hook<Function>::original() const noexcept {
    if constexpr (std::is_same_v<Function, Unstable>)
    {
        return reinterpret_cast<Function>(nullptr);
    }

    return _targetFunction;
}

template<typename Function>
Function Hook<Function>::detour() const noexcept {
    if constexpr (std::is_same_v<Function, Unstable>)
    {
        return reinterpret_cast<Function>(nullptr);
    }

    return _detourFunction;
}

template<typename Function>
std::any Hook<Function>::unstableOriginal() const noexcept {
    return _unstableTarget;
}

template<typename Function>
std::any Hook<Function>::unstableDetour() const noexcept {
    return _unstableTarget;
}

}
