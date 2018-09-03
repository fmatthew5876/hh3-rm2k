#pragma once

#include <gsl/span>
#include <type_traits>

template <typename T>
using Span = gsl::span<T>;

template <typename T>
auto makeSpan(T* p, size_t n) {
    return Span<std::remove_reference_t<T>>(p, n);
}
