#pragma once
#include <type_traits>

template <typename T>
struct is_a3d_channel : std::is_base_of<A3d_Channel, T> {
};

template <typename T>
constexpr bool is_a3d_channel_v = is_a3d_channel<T>::value;