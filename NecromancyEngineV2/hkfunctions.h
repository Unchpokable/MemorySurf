#pragma once

#include "typedefs.h"

namespace necromancy::hooks {

using namespace necromancy::Typedefs;

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
    Function get(const std::string& name) const;

    bool allValid() const noexcept;

private:
    void copyWithReferenceInc(const HkFunctions& other);

    HkFunctions& add(const std::string& key, void* func);

    static int _referenceCount;
    static HkFunctions* _instance;

    std::unordered_map<std::string, void*> _functions;
};

template<typename Function>
Function HkFunctions::get(const std::string& name) const {
    return reinterpret_cast<Function>(_functions.at(name));
}

}
