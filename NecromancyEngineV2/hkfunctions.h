#pragma once

#include "q3ddetours.h"
#include "typedefs.h"

namespace Necromancy {
namespace Detours {

using namespace Necromancy::Typedefs;

class HkFunctions
{
public:
    HkFunctions() noexcept;
    ~HkFunctions() noexcept;

    HkFunctions(const HkFunctions& other) noexcept;
    HkFunctions(HkFunctions&& other) noexcept;

    static HkFunctions setup();

    template<typename Function>
    Function get(const std::string &function) const;

private:
    static int _referenceCount;
    static HkFunctions* _instance;

    std::unordered_map<std::string, std::any> _functions;
};

template<typename Function>
Function HkFunctions::get(const std::string &function) const {
    return std::any_cast<Function>(_functions.at(function));
}

}
}