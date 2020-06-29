#pragma once

#include <pybind11/pybind11.h>

template<typename T, unsigned Size>
pybind11::tuple zero_tuple()
{
    // TODO - use std::fill
    const auto result { pybind11::tuple(Size) };
    for (auto i {0u}; i < Size; i++)
    {
        result[i] = static_cast<T>(0);
    }
    return result;
}
