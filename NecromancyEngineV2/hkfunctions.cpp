#include "pch.h"
#include "hkfunctions.h"

using namespace Necromancy::Detours;

HkFunctions* HkFunctions::_instance = nullptr;
int HkFunctions::_referenceCount = 0;

HkFunctions::HkFunctions() noexcept {
    ++_referenceCount;
}

HkFunctions::~HkFunctions() noexcept {
    --_referenceCount;

    if(_referenceCount == 0)
    {
        delete _instance;
    }
}

HkFunctions::HkFunctions(const HkFunctions &other) noexcept {
    ++_referenceCount;

    _functions = other._functions;
}

HkFunctions::HkFunctions(HkFunctions &&other) noexcept {
    ++_referenceCount;
    _functions = other._functions;
}

HkFunctions HkFunctions::setup() {
    if(_instance)
    {
        _referenceCount++;
        return *_instance;
    }

    auto result = new HkFunctions();

    result->_functions["TrueCallChannel"] = 
        reinterpret_cast<void(__thiscall*)(A3d_Channel*)>(DetourFindFunction("highpoly.dll", "?CallChannel@A3d_Channel@@UAEXXZ"));

    result->_functions["Aco_FloatChannel_GetFloat"] =
        reinterpret_cast<Aco_FloatChannel_GetFloat>(DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll",
                                                                  "?GetFloat@Aco_FloatChannel@@UAEMXZ"));
    result->_functions["Aco_FloatChannel_GetDefaultFloat"] =
        reinterpret_cast<Aco_FloatChannel_GetDefaultFloat>(DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll",
                                                                         "?GetDefaultFloat@Aco_FloatChannel@@UAEMXZ"));
    result->_functions["Aco_FloatChannel_SetFloat"] =
        reinterpret_cast<Aco_FloatChannel_SetFloat>(DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll",
                                                                  "?SetFloat@Aco_FloatChannel@@UAEXM@Z"));

    result->_functions["Aco_ArrayTable_GetTable"] =
        reinterpret_cast<Aco_ArrayTable_GetTable>(DetourFindFunction("6918910A-F8BA-43C4-B8D4-CD6587D0F67C.dll",
                                                                "?GetTable@Aco_Array_Table@@UAEPAVARRAY_TABLE_NEW@@XZ"));

    _instance = result;

    return *_instance;
}
