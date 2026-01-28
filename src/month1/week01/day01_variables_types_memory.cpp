#include "cpp_mastery.hpp"
#include <cstdint>
#include <climits>

// Day 1: Variables, Types, and Memory Layout
// Learning Goals:
// - Understand fundamental types and their sizes
// - Learn about memory layout and alignment
// - Explore initialization vs assignment
// - Measure performance implications of different types

TEST_CASE("Day 1: Fundamental Type Sizes", "[day01][types]") {
    
    SECTION("Type size verification") {
        cpp_mastery::Logger::info("=== Fundamental Type Sizes ===");
        
        INFO("char: " << sizeof(char) << " bytes");
        INFO("short: " << sizeof(short) << " bytes");
        INFO("int: " << sizeof(int) << " bytes");
        INFO("long: " << sizeof(long) << " bytes");
        INFO("long long: " << sizeof(long long) << " bytes");
        INFO("float: " << sizeof(float) << " bytes");
        INFO("double: " << sizeof(double) << " bytes");
        INFO("pointer: " << sizeof(void*) << " bytes");
        
        // Verify C++ standard guarantees
        REQUIRE(sizeof(char) == 1);
        REQUIRE(sizeof(short) >= 2);
        REQUIRE(sizeof(int) >= 2);
        REQUIRE(sizeof(long) >= 4);
        REQUIRE(sizeof(long long) >= 8);
        REQUIRE(sizeof(float) >= 4);
        REQUIRE(sizeof(double) >= 8);
        
        // Test type ranges
        REQUIRE(CHAR_BIT >= 8);
        REQUIRE(INT_MAX >= 32767);
        
        cpp_mastery::Logger::info("All type size guarantees verified");
    }
    
    SECTION("Fixed-width integer types") {
        // C++11 fixed-width types
        REQUIRE(sizeof(std::int8_t) == 1);
        REQUIRE(sizeof(std::int16_t) == 2);
        REQUIRE(sizeof(std::int32_t) == 4);
        REQUIRE(sizeof(std::int64_t) == 8);
        
        REQUIRE(sizeof(std::uint8_t) == 1);
        REQUIRE(sizeof(std::uint16_t) == 2);
        REQUIRE(sizeof(std::uint32_t) == 4);
        REQUIRE(sizeof(std::uint64_t) == 8);
        
        cpp_mastery::Logger::info("Fixed-width integer types verified");
    }
}

TEST_CASE("Day 1: Memory Alignment and Padding", "[day01][memory]") {
    
    SECTION("Structure padding demonstration") {
        struct UnalignedStruct {
            char c;      // 1 byte
            int i;       // 4 bytes (likely padded)
            char c2;     // 1 byte
            double d;    // 8 bytes (likely padded)
        };
        
        struct AlignedStruct {
            double d;    // 8 bytes
            int i;       // 4 bytes
            char c;      // 1 byte
            char c2;     // 1 byte
            // 2 bytes padding to align to 8-byte boundary
        };
        
        INFO("UnalignedStruct size: " << sizeof(UnalignedStruct) << " bytes");
        INFO("AlignedStruct size: " << sizeof(AlignedStruct) << " bytes");
        INFO("Sum of UnalignedStruct members: " << (sizeof(char) + sizeof(int) + sizeof(char) + sizeof(double)) << " bytes");
        
        // Alignment usually causes padding
        REQUIRE(sizeof(UnalignedStruct) >= sizeof(char) + sizeof(int) + sizeof(char) + sizeof(double));
        REQUIRE(sizeof(AlignedStruct) >= sizeof(double) + sizeof(int) + sizeof(char) + sizeof(char));
        
        // Better aligned struct is often smaller or same size
        REQUIRE(sizeof(AlignedStruct) < sizeof(UnalignedStruct));
        
        cpp_mastery::Logger::info("Memory alignment and padding demonstrated");
    }
    
    SECTION("Alignment requirements") {
        // Test alignment of different types
        alignas(16) int aligned_int;
        alignas(32) double aligned_double;
        
        REQUIRE(reinterpret_cast<uintptr_t>(&aligned_int) % 16 == 0);
        REQUIRE(reinterpret_cast<uintptr_t>(&aligned_double) % 32 == 0);
        
        cpp_mastery::Logger::info("Custom alignment requirements work");
    }
}

TEST_CASE("Day 1: Variable Initialization", "[day01][initialization]") {
    
    SECTION("Different initialization syntaxes") {
        // Direct initialization
        int a(42);
        REQUIRE(a == 42);
        
        // Copy initialization
        int b = 42;
        REQUIRE(b == 42);
        
        // List initialization (C++11)
        int c{42};
        REQUIRE(c == 42);
        
        // Default initialization
        int d{};  // Zero-initialized
        REQUIRE(d == 0);
        
        // Assignment after declaration
        int e;
        e = 42;
        REQUIRE(e == 42);
        
        cpp_mastery::Logger::info("All initialization methods work correctly");
    }
    
    SECTION("Array initialization") {
        // C-style array initialization
        int arr1[5] = {1, 2, 3, 4, 5};
        REQUIRE(arr1[0] == 1);
        REQUIRE(arr1[4] == 5);
        
        // Partial initialization (rest zero-initialized)
        int arr2[5] = {1, 2};
        REQUIRE(arr2[0] == 1);
        REQUIRE(arr2[1] == 2);
        REQUIRE(arr2[2] == 0);
        REQUIRE(arr2[4] == 0);
        
        // Zero initialization
        int arr3[5] = {};
        for (int i = 0; i < 5; ++i) {
            REQUIRE(arr3[i] == 0);
        }
        
        cpp_mastery::Logger::info("Array initialization patterns work");
    }
}

TEST_CASE("Day 1: Stack vs Heap Allocation", "[day01][memory]") {
    
    SECTION("Stack allocation characteristics") {
        // Stack allocation - automatic storage
        const size_t size = 1000;
        int stack_array[size];
        
        // Initialize array
        for (size_t i = 0; i < size; ++i) {
            stack_array[i] = static_cast<int>(i);
        }
        
        // Verify values
        REQUIRE(stack_array[0] == 0);
        REQUIRE(stack_array[999] == 999);
        
        cpp_mastery::Logger::info("Stack allocation works correctly");
        // Automatically cleaned up when scope ends
    }
    
    SECTION("Heap allocation characteristics") {
        // Heap allocation - dynamic storage
        const size_t size = 1000;
        int* heap_array = new int[size];
        
        // Initialize array
        for (size_t i = 0; i < size; ++i) {
            heap_array[i] = static_cast<int>(i);
        }
        
        // Verify values
        REQUIRE(heap_array[0] == 0);
        REQUIRE(heap_array[999] == 999);
        
        delete[] heap_array;  // Manual cleanup required
        
        cpp_mastery::Logger::info("Heap allocation works correctly");
    }
}

TEST_CASE("Day 1: Performance Comparisons", "[day01][benchmark]") {
    const size_t iterations = 1000000;
    
    BENCHMARK("char arithmetic") {
        volatile char a = 1;
        volatile char b = 2;
        volatile char result = a + b;
        cpp_mastery::do_not_optimize(result);
        return result;
    };
    
    BENCHMARK("int arithmetic") {
        volatile int a = 1;
        volatile int b = 2;
        volatile int result = a + b;
        cpp_mastery::do_not_optimize(result);
        return result;
    };
    
    BENCHMARK("long long arithmetic") {
        volatile long long a = 1;
        volatile long long b = 2;
        volatile long long result = a + b;
        cpp_mastery::do_not_optimize(result);
        return result;
    };
    
    BENCHMARK("float arithmetic") {
        volatile float a = 1.0f;
        volatile float b = 2.0f;
        volatile float result = a + b;
        cpp_mastery::do_not_optimize(result);
        return result;
    };
    
    BENCHMARK("double arithmetic") {
        volatile double a = 1.0;
        volatile double b = 2.0;
        volatile double result = a + b;
        cpp_mastery::do_not_optimize(result);
        return result;
    };
}

TEST_CASE("Day 1: Memory Access Patterns", "[day01][benchmark]") {
    const size_t size = 100000;
    auto data = cpp_mastery::generate_sequential_data<int>(size);
    
    BENCHMARK("Sequential memory access") {
        long long sum = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };
    
    BENCHMARK("Random memory access") {
        long long sum = 0;
        std::random_device rd;
        std::mt19937 gen(42);
        std::uniform_int_distribution<size_t> dis(0, data.size() - 1);
        
        for (int i = 0; i < 10000; ++i) {
            sum += data[dis(gen)];
        }
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };
}

TEST_CASE("Day 1: Variable Scope and Lifetime", "[day01][scope]") {
    
    SECTION("Scope rules verification") {
        int global_var = 100;  // Function scope
        
        {
            int local_var = 200;  // Block scope
            REQUIRE(local_var == 200);
            
            {
                int nested_var = 300;  // Nested block scope
                REQUIRE(nested_var == 300);
                REQUIRE(local_var == 200);  // Still accessible
                REQUIRE(global_var == 100); // Still accessible
            }
            // nested_var is no longer accessible here
            
            REQUIRE(local_var == 200);
            REQUIRE(global_var == 100);
        }
        // local_var is no longer accessible here
        
        REQUIRE(global_var == 100);
        
        cpp_mastery::Logger::info("Variable scope rules work correctly");
    }
    
    SECTION("Static variable lifetime") {
        auto get_counter = []() {
            static int counter = 0;
            return ++counter;
        };
        
        REQUIRE(get_counter() == 1);
        REQUIRE(get_counter() == 2);
        REQUIRE(get_counter() == 3);
        
        cpp_mastery::Logger::info("Static variable lifetime demonstrated");
    }
}

TEST_CASE("Testing addition, multiplication, and division", "[day01][benchmark]") {
    int size = 1000;
    float size_f = 999.9;
    std::random_device rd;
    std::mt19937 gen(42);
    std::uniform_int_distribution<size_t> dis(1, 100);
    
    BENCHMARK("int addition") {
        int sum = dis(gen);
        for (int i = 0; i < size; ++i) {
            sum += i;
        }
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };

    BENCHMARK("int multiplication") {
        int product = dis(gen);
        for (int i = 1; i < size; ++i) {
            product *= i;
        }
        cpp_mastery::do_not_optimize(product);
        return product;
    };

    BENCHMARK("int division") {
        int total = dis(gen);
        for (int i = 1; i <= size; ++i) {
            total /= i;
        }
        cpp_mastery::do_not_optimize(total);
        return total;
    };

    BENCHMARK("float addition") {
        float sum = 0.0;
        for (float i = 0.1; i < size_f; ++i) {
            sum += i;
        }
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };

    BENCHMARK("float multiplication") {
        float product = 1.0;
        for (float i = 0.1; i < size_f; ++i) {
            product *= i;
        }
        cpp_mastery::do_not_optimize(product);
        return product;
    };

    BENCHMARK("float division") {
        float total = 65535.2;
        for (float i = 0.1; i < size_f; ++i) {
            total /= i;
        }
        cpp_mastery::do_not_optimize(total);
        return total;
    };
    
}

// Day 1 Summary:
// - Fundamental types have guaranteed minimum sizes, actual sizes are platform-dependent
// - Memory alignment affects struct sizes and can impact performance
// - Different initialization syntaxes have same runtime performance
// - Stack allocation is faster than heap allocation for small objects
// - Sequential memory access is much faster than random access due to cache behavior
// - Variable scope determines lifetime and accessibility
// - Understanding these fundamentals is crucial for writing efficient C++ code