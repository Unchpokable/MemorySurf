#include "pch.h"

#include "hook.h"

using namespace necromancy::hooks;

Hook::Hook(void* target, void* detour)
    : _targetFunction(target), _detourFunction(detour), _hookAttached(false) {
    // empty
}

Hook::Hook() : _targetFunction(nullptr), _detourFunction(nullptr), _hookAttached(false) {
    // empty
}

Hook::~Hook() {
    detach();
}

Status Hook::attach() {
    if(_hookAttached)
        return Status::AlreadyInUse;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if(DetourAttach(&_targetFunction, _detourFunction) != NO_ERROR) {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _hookAttached = true;
    return Status::Ok;
}

Status Hook::detach() {
    if(!_hookAttached)
        return Status::UnnecessaryOperation;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if(DetourDetach(&_targetFunction, _detourFunction) != NO_ERROR) {
        DetourTransactionAbort();
        return Status::DetourException;
    }

    DetourTransactionCommit();
    _hookAttached = false;
    return Status::Ok;
}
