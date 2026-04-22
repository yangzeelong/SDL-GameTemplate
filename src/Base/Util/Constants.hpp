#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

// ============================================
// Mathematical and Physical Constants
// ============================================
#include <cstdint>

namespace Constants {

// Time conversions
constexpr uint64_t NANO_PER_SEC = 1'000'000'000ULL; // Nanoseconds per second
constexpr uint64_t NANO_PER_MILLI = 1'000'000ULL; // Nanoseconds per millisecond
constexpr uint64_t MICRO_PER_SEC = 1'000'000ULL; // Microseconds per second
constexpr uint64_t MILLI_PER_SEC = 1'000ULL; // Milliseconds per second

} // namespace Constants

#endif // CONSTANTS_HPP_
