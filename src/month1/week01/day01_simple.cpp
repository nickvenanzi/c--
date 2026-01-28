#include "cpp_mastery.hpp"
#include <cstdint>
#include <climits>
// Day 1: Variables, Types, and Memory Layout - Simple Version

TEST_CASE("Day 1: Fundamental Type Sizes", "[day01][types]") {
    cpp_mastery::Logger::info("=== Fundamental Type Sizes ===");
    
    // Verify minimum guarantees
    REQUIRE(sizeof(char) == 1);
    REQUIRE(sizeof(int) >= 2);
    REQUIRE(sizeof(long) >= 4);
    REQUIRE(sizeof(float) >= 4);
    REQUIRE(sizeof(double) >= 8);
    
    cpp_mastery::Logger::info("Type size verification complete");
}

TEST_CASE("Day 1: Basic Arithmetic", "[day01][arithmetic]") {
    int a = 10;
    int b = 20;
    int sum = a + b;
    
    REQUIRE(sum == 30);
    
    double x = 3.14;
    double y = 2.86;
    double result = x + y;
    
    REQUIRE(result == Approx(6.0));  // Approximate equality for floats
    
    cpp_mastery::Logger::info("Basic arithmetic works correctly");
}

TEST_CASE("Day 1: Initialization Methods", "[day01][initialization]") {
    // Different ways to initialize variables
    int a(42);      // Direct initialization
    int b = 42;     // Copy initialization  
    int c{42};      // List initialization (C++11)
    int d{};        // Zero initialization
    
    REQUIRE(a == 42);
    REQUIRE(b == 42);
    REQUIRE(c == 42);
    REQUIRE(d == 0);
    
    cpp_mastery::Logger::info("All initialization methods work");
}

TEST_CASE("Day 1: Memory Layout Basics", "[day01][memory]") {
    struct SimpleStruct {
        char c;
        int i;
        double d;
    };
    SimpleStruct myStruct;
    SimpleStruct myStruct2;
    
    REQUIRE(sizeof(SimpleStruct) == 16);
    REQUIRE((char *)&myStruct.i - (char *)&myStruct == 4);
    REQUIRE((char *)&myStruct.d - (char *)&myStruct == 8);
    REQUIRE((char *)&myStruct2 - (char *)&myStruct == -16);
    // Usually there's padding for alignment
    REQUIRE(sizeof(SimpleStruct) >= sizeof(char) + sizeof(int) + sizeof(double));
    
    cpp_mastery::Logger::info("Memory layout demonstration complete");
}

TEST_CASE("Day 1: Array Basics", "[day01][arrays]") {
    // Stack array
    int stack_array[5] = {1, 2, 3, 4, 5};
    REQUIRE(stack_array[0] == 1);
    REQUIRE(stack_array[4] == 5);
    
    // Dynamic array
    int* heap_array = new int[5];
    for (int i = 0; i < 5; ++i) {
        heap_array[i] = i + 1;
    }
    
    REQUIRE(heap_array[0] == 1);
    REQUIRE(heap_array[4] == 5);
    
    delete[] heap_array;
    
    cpp_mastery::Logger::info("Array operations work correctly");
}

TEST_CASE("Day 1: Stack vs Heap Performance", "[day01][benchmark]") {
    const size_t array_size = 1000;
    
    BENCHMARK("Stack allocation") {
        // Stack allocation - automatic storage
        int stack_array[1000];  // Note: Fixed size required for stack arrays
        
        // Initialize array
        for (size_t i = 0; i < array_size; ++i) {
            stack_array[i] = static_cast<int>(i);
        }
        
        // Use the array to prevent optimization
        volatile int sum = 0;
        for (size_t i = 0; i < array_size; ++i) {
            sum += stack_array[i];
        }
        
        cpp_mastery::do_not_optimize(sum);
        return sum;
        // Stack array automatically destroyed when scope ends
    };
    
    BENCHMARK("Heap allocation") {
        // Heap allocation - dynamic storage
        int* heap_array = new int[array_size];
        
        // Initialize array
        for (size_t i = 0; i < array_size; ++i) {
            heap_array[i] = static_cast<int>(i);
        }
        
        // Use the array to prevent optimization
        volatile int sum = 0;
        for (size_t i = 0; i < array_size; ++i) {
            sum += heap_array[i];
        }
        
        delete[] heap_array;  // Manual cleanup required
        
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };
    
    BENCHMARK("std::vector (heap-based)") {
        // std::vector - heap-based but with RAII
        std::vector<int> vec(array_size);
        
        // Initialize vector
        for (size_t i = 0; i < array_size; ++i) {
            vec[i] = static_cast<int>(i);
        }
        
        // Use the vector to prevent optimization
        volatile int sum = 0;
        for (size_t i = 0; i < array_size; ++i) {
            sum += vec[i];
        }
        
        cpp_mastery::do_not_optimize(sum);
        return sum;
        // Vector automatically cleaned up (RAII)
    };
}

TEST_CASE("Day 1: Simple Array Performance Test", "[day01][benchmark]") {
    BENCHMARK("Arrays on the Stack") {
        volatile int sum = 0;  // Make sum volatile
        for (int i = 0; i < 100; ++i) {
            int myVec[50];
            
            // Force the compiler to actually allocate and use the array
            cpp_mastery::do_not_optimize(myVec);  // Prevent array optimization
        }
        return sum;
    };

    BENCHMARK("Arrays on the Heap") {
        volatile int sum = 0;  // Make sum volatile
        for (int i = 0; i < 100; ++i) {
            int *myVec = new int[50];
            
            // Force the compiler to actually allocate and use the array
            cpp_mastery::do_not_optimize(myVec);  // Prevent pointer optimization
            
            delete[] myVec;
        }
        return sum;
    };
}

TEST_CASE("Day 1: Simple Performance Test", "[day01][benchmark]") {
    const int size = 100000;
    auto data = cpp_mastery::generate_sequential_data<int>(size);
    
    BENCHMARK("Sum calculation") {
        long long sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += data[i];
        }
        cpp_mastery::do_not_optimize(sum);
        return sum;
    };
    
    cpp_mastery::Logger::info("Performance test complete");
}

// Day 1 Summary:
// - Learned about fundamental type sizes and guarantees
// - Explored different initialization syntaxes
// - Understood basic memory layout and padding
// - Practiced with arrays and dynamic allocation
// - Measured simple performance characteristics