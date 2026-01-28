#pragma once

// Catch2 configuration for testing and benchmarking
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/catch.hpp"

// Standard library includes for common functionality
#include <chrono>
#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <numeric>
#include <memory>
#include <string>
#include <iostream>
#include <type_traits>
#include <functional>

namespace cpp_mastery {

// ============================================================================
// Data Generation Utilities
// ============================================================================

template<typename T>
std::vector<T> generate_random_data(size_t size, T min_val = T{}, T max_val = T{100}) {
    std::vector<T> data(size);
    std::random_device rd;
    std::mt19937 gen(42);  // Fixed seed for reproducible benchmarks
    
    // C++14 compatible version
    if (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> dis(min_val, max_val);
        std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
    } else {
        std::uniform_real_distribution<T> dis(min_val, max_val);
        std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
    }
    
    return data;
}

template<typename T>
std::vector<T> generate_sequential_data(size_t size, T start = T{}) {
    std::vector<T> data(size);
    std::iota(data.begin(), data.end(), start);
    return data;
}

template<typename T>
std::vector<T> generate_reverse_data(size_t size, T start = T{}) {
    auto data = generate_sequential_data(size, start);
    std::reverse(data.begin(), data.end());
    return data;
}

// ============================================================================
// Performance Utilities
// ============================================================================

// Prevent compiler optimization of unused results
template<typename T>
void do_not_optimize(T&& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// Force memory barrier
inline void memory_barrier() {
    asm volatile("" : : : "memory");
}

// ============================================================================
// Memory Utilities
// ============================================================================

// Get cache line size (typical value, can be refined per platform)
constexpr size_t CACHE_LINE_SIZE = 64;

// Align value to cache line boundary
constexpr size_t align_to_cache_line(size_t value) {
    return (value + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1);
}

// Check if pointer is aligned to boundary
template<size_t Alignment>
bool is_aligned(const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr) % Alignment == 0;
}

// ============================================================================
// Type Utilities (C++14 compatible)
// ============================================================================

// Compile-time type name
template<typename T>
constexpr const char* type_name() {
    return std::is_same<T, int>::value ? "int" :
           std::is_same<T, double>::value ? "double" :
           std::is_same<T, float>::value ? "float" :
           std::is_same<T, char>::value ? "char" :
           std::is_same<T, bool>::value ? "bool" : "unknown";
}

// ============================================================================
// Testing Utilities
// ============================================================================

// Verify two containers have same content
template<typename Container1, typename Container2>
bool containers_equal(const Container1& c1, const Container2& c2) {
    if (c1.size() != c2.size()) return false;
    return std::equal(c1.begin(), c1.end(), c2.begin());
}

// Generate test cases for different sizes
inline std::vector<size_t> test_sizes() {
    return {10, 100, 1000, 10000, 100000};
}

// Generate benchmark sizes (powers of 2 for cache analysis)
inline std::vector<size_t> benchmark_sizes() {
    return {64, 256, 1024, 4096, 16384, 65536};
}

// ============================================================================
// Logging and Output
// ============================================================================

class Logger {
public:
    enum Level { DEBUG, INFO, BENCHMARK, ERROR };
    
    static void log(Level level, const std::string& message) {
        const char* prefix[] = {"[DEBUG]", "[INFO]", "[BENCH]", "[ERROR]"};
        std::cout << prefix[level] << " " << message << std::endl;
    }
    
    static void info(const std::string& message) { log(INFO, message); }
    static void debug(const std::string& message) { log(DEBUG, message); }
    static void benchmark(const std::string& message) { log(BENCHMARK, message); }
    static void error(const std::string& message) { log(ERROR, message); }
};

} // namespace cpp_mastery