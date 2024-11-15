#pragma once

#include "hkfunctions.h"
#include <type_traits>

namespace Necromancy {
namespace Memory {

using namespace Necromancy::Detours;

#define DECLARE_BASE_TYPE(T) \
    typedef T BaseType; \
    using BaseType##_t = typename std::conditional_t< \
        std::is_class<T>::value || std::is_union<T>::value, \
        T, \
        std::type_identity<T>>::type;

class Q3DChannelReader {
public:
    Q3DChannelReader(A3d_Channel* target);
    virtual ~Q3DChannelReader() = default;

    Q3DChannelReader(const Q3DChannelReader& other) noexcept = default;
    Q3DChannelReader(Q3DChannelReader&& other) noexcept = default;

    Q3DChannelReader& operator=(const Q3DChannelReader& other) noexcept = default;
    Q3DChannelReader& operator=(Q3DChannelReader&& other) noexcept = default;

protected:
    HkFunctions _functions;
    A3d_Channel* _target;
};

}
}


