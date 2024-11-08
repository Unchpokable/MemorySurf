#pragma once

enum class Status {
    AlreadyInUse, UnnecessaryOperation, DetourException, Ok
};

template<typename Function>
class Hook final
{
public:

    Hook(Function target, Function detour);
    ~Hook();

    Status attach();
    Status detach();

    Function original() const noexcept;
    Function detour() const noexcept;

private:
    Function _targetFunction;
    Function _detourFunction;

    bool _hookAttached;
};

template<typename Function>
Hook<Function>::Hook(Function target, Function detour) : _targetFunction(target), _detourFunction(detour), _hookAttached(false)
{
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
Function Hook<Function>::original() const noexcept {
    return _targetFunction;
}

template<typename Function>
Function Hook<Function>::detour() const noexcept {
    return _detourFunction;
}
