#pragma once

#include <cstdint>
#include <functional>

#define DEFINE_ID_HASH(IDType) \
template<> \
struct std::hash<IDType> { \
    std::size_t operator()(const IDType& id) const noexcept { \
        return std::hash<uint32_t>{}(id.value); \
    } \
}; \

namespace core {

/// <summary>
/// Generic ID base class using CRTP for type safety.
/// </summary>
/// <typeparam name="Derived"></typeparam>
template<typename Derived>
struct ID {
    uint32_t value = 0;

    ID() = default;

    explicit ID(uint32_t v) : value(v) {}

    // Implicit conversion to uint32_t
    operator uint32_t() const { return value; }

    // Equality operators
    bool operator==(const Derived& other) const {
        return value == other.value;
    }

    bool operator!=(const Derived& other) const {
        return value != other.value;
    }

    // less-than for use in ordered containers
    bool operator<(const Derived& other) const {
        return value < other.value;
    }
};

}

