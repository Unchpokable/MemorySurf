#pragma once

#include "hkfunctions.h"
#include <type_traits>

#include "q3dprimitivereader.h"

namespace Necromancy::Memory {

class Q3DChannelReader;

using namespace Necromancy::Detours;

template <typename T>
concept ValidPrimitiveReader =
    std::is_base_of_v<Q3DChannelReader, T> &&
    requires {
        typename T::DataType;
        requires Primitive<typename T::DataType>;
        requires std::is_base_of_v<Q3DPrimitiveReader<typename T::DataType>, T>;
    };

class Q3DChannelReader {
public:
    Q3DChannelReader(A3d_Channel* target);
    virtual ~Q3DChannelReader() = default;

    Q3DChannelReader(const Q3DChannelReader& other) noexcept;
    Q3DChannelReader(Q3DChannelReader&& other) noexcept;

    Q3DChannelReader& operator=(const Q3DChannelReader& other) noexcept;
    Q3DChannelReader& operator=(Q3DChannelReader&& other) noexcept;

protected:
    HkFunctions _functions;
    A3d_Channel* _target;
};

}


