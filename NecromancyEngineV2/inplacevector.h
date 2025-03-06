#pragma once

#include <assert.h>
#include <cstddef>

template<typename T, std::size_t Size = 20>
struct InPlaceVector final {
    static_assert(std::is_trivially_copyable_v<T>);

    explicit InPlaceVector() { used = 0; }

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    void append(const T& item);
    void erase();

    void clear();

    T data[Size];
    std::size_t used;
};

template<typename T, std::size_t Size>
T& InPlaceVector<T, Size>::operator[](std::size_t index) {
    assert(index < used);
    return data[index];
}

template<typename T, std::size_t Size>
const T& InPlaceVector<T, Size>::operator[](std::size_t index) const {
    return const_cast<InPlaceVector*>(this)->operator[](index);
}

template <typename T, std::size_t Size>
void InPlaceVector<T, Size>::append(const T& item) {
    assert(used < Size);
    data[used++] = item;
}

template <typename T, std::size_t Size>
void InPlaceVector<T, Size>::erase() {
    used--;
}

template <typename T, std::size_t Size>
void InPlaceVector<T, Size>::clear() {
    used = 0;
}
