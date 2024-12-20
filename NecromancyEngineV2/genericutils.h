#pragma once

template<typename T>
    requires !std::is_pointer_v<T> && !std::is_reference_v<T>
constexpr bool notNull(T* ptr) {
    return ptr != nullptr;
}
