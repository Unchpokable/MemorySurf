#pragma once

#include "pch.h"

template<typename T>
using is_pod = std::is_pod<T>;

template<typename T, typename = void>
struct is_simple_struct : std::false_type {};

template<typename T>
struct is_simple_struct<T, std::enable_if_t<is_pod<T>::value>> : std::true_type {};

template<typename T>
constexpr bool is_simple_struct_v = is_simple_struct<T>::value;