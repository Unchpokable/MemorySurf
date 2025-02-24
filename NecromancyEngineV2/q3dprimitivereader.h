#pragma once

namespace necromancy::Memory {

template<typename T>
concept Primitive = std::is_fundamental_v<T>;

template<Primitive T>
class Q3DPrimitiveReader {
public:
    using DataType = T;
    using DataTypePtr = T*;
    using DataTypeRef = T&;

    Q3DPrimitiveReader() = default;
    virtual ~Q3DPrimitiveReader() = default;

    virtual T get() const = 0;
    virtual void set(T value) const = 0;
};

}
