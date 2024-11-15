#pragma once

namespace Necromancy {
namespace Memory {

template<typename T>
class Q3DPrimitiveReader {
    virtual T get() const = 0;
    virtual void set(T value) const = 0;
};

}
}
