#pragma once

namespace Necromancy {
namespace Memory {

template<typename T>
class Q3DPrimitiveReader {
    using DataType = T;
    using DataTypePtr = T*;
    using DataTypeRef = T&;

    virtual T get() const = 0;
    virtual void set(T value) const = 0;
};

}
}
