#pragma once

#include "typedefs.h"

namespace Necromancy::Detours {

using namespace Necromancy::Typedefs;

class HkFunctions
{
public:
    HkFunctions() noexcept;
    ~HkFunctions() noexcept;

    HkFunctions(const HkFunctions& other) noexcept;
    HkFunctions(HkFunctions&& other) noexcept;

    HkFunctions& operator=(const HkFunctions& other) noexcept;
    HkFunctions& operator=(HkFunctions&& other) noexcept;

    static HkFunctions setup();

    template<typename Function>
    Function get() const;

private:
    void copyWithReferenceInc(const HkFunctions& other);

    HkFunctions& add(const std::string& key, const std::any& value);

    static int _referenceCount;
    static HkFunctions* _instance;

    std::unordered_map<std::string, std::any> _functions;
};

template<typename Function>
Function HkFunctions::get() const {
    return std::any_cast<Function>(_functions.at(typeid(Function).name()));
}

}
