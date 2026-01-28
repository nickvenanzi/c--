#include "cpp_mastery.hpp"
#include <cstdint>

// =============================================================================
// DAY 2: Pointers and References Deep Dive
// =============================================================================
// Topics Covered:
// - Pointer arithmetic and memory addresses
// - Reference semantics and aliasing  
// - Pointer vs reference performance
// - Common pointer pitfalls and debugging
// =============================================================================

TEST_CASE("Pointer Fundamentals", "[pointers][day2]") {
    
    SECTION("Basic pointer operations") {
        int value = 42;
        int* ptr = &value;
        
        std::cerr << "\n=== Basic Pointer Operations ===" << std::endl;
        std::cerr << "value: " << value << std::endl;
        std::cerr << "value address: " << &value << std::endl;
        std::cerr << "ptr: " << ptr << std::endl;
        std::cerr << "*ptr: " << *ptr << std::endl;
        std::cerr << "ptr address: " << &ptr << std::endl;
        
        // Verify pointer relationships
        REQUIRE(ptr == &value);
        REQUIRE(*ptr == value);
        REQUIRE(*ptr == 42);
        
        // Modify through pointer
        *ptr = 100;
        REQUIRE(value == 100);
        REQUIRE(*ptr == 100);
        
        cpp_mastery::Logger::info("Basic pointer operations verified");
    }
    
    SECTION("Pointer arithmetic and memory layout") {
        int array[5] = {10, 20, 30, 40, 50};
        int* ptr = array;  // Points to first element
        
        std::cerr << "\n=== Pointer Arithmetic ===" << std::endl;
        
        for (int i = 0; i < 5; ++i) {
            std::cerr << "array[" << i << "] = " << array[i] 
                     << " at address " << &array[i] << std::endl;
            std::cerr << "ptr + " << i << " = " << *(ptr + i) 
                     << " at address " << (ptr + i) << std::endl;
            
            // Verify pointer arithmetic
            REQUIRE(ptr + i == &array[i]);
            REQUIRE(*(ptr + i) == array[i]);
        }
        
        // Address differences
        std::cerr << "Address difference between elements: " 
                 << (ptr + 1) - ptr << " * sizeof(int) = " 
                 << ((ptr + 1) - ptr) * sizeof(int) << " bytes" << std::endl;
        
        REQUIRE((ptr + 1) - ptr == 1);  // Pointer arithmetic in elements
        REQUIRE(reinterpret_cast<char*>(ptr + 1) - reinterpret_cast<char*>(ptr) == sizeof(int));
        
        cpp_mastery::Logger::info("Pointer arithmetic verified");
    }
    
    SECTION("Null and invalid pointers") {
        int* null_ptr = nullptr;
        int* invalid_ptr = reinterpret_cast<int*>(0xDEADBEEF);
        
        std::cerr << "\n=== Null and Invalid Pointers ===" << std::endl;
        std::cerr << "null_ptr: " << null_ptr << std::endl;
        std::cerr << "invalid_ptr: " << invalid_ptr << std::endl;
        
        // Safe null pointer checks
        REQUIRE(null_ptr == nullptr);
        REQUIRE(!null_ptr);  // Implicit conversion to bool
        
        if (null_ptr) {
            // This should never execute
            REQUIRE(false);
        }
        
        cpp_mastery::Logger::info("Null pointer safety verified");
    }
}

TEST_CASE("Reference Fundamentals", "[references][day2]") {
    
    SECTION("Reference basics and aliasing") {
        int original = 42;
        int& ref = original;  // Reference must be initialized
        
        std::cerr << "\n=== Reference Basics ===" << std::endl;
        std::cerr << "original: " << original << " at " << &original << std::endl;
        std::cerr << "ref: " << ref << " at " << &ref << std::endl;
        
        // References are aliases - same address
        REQUIRE(&ref == &original);
        REQUIRE(ref == original);
        
        // Modifying through reference
        ref = 100;
        REQUIRE(original == 100);
        REQUIRE(ref == 100);
        
        // Modifying original
        original = 200;
        REQUIRE(ref == 200);
        
        cpp_mastery::Logger::info("Reference aliasing verified");
    }
    
    SECTION("References vs pointers syntax") {
        int value = 42;
        
        // Pointer version
        int* ptr = &value;
        *ptr = 100;
        
        // Reference version  
        int& ref = value;
        ref = 200;
        
        std::cerr << "\n=== Reference vs Pointer Syntax ===" << std::endl;
        std::cerr << "After pointer modification: " << value << std::endl;
        std::cerr << "After reference modification: " << value << std::endl;
        
        REQUIRE(value == 200);
        
        cpp_mastery::Logger::info("Reference vs pointer syntax compared");
    }
    
    SECTION("Reference limitations") {
        int a = 10, b = 20;
        int& ref = a;  // Must initialize
        
        // Cannot reassign references
        // ref = b;  // This assigns b's VALUE to a, doesn't make ref refer to b
        ref = b;
        
        std::cerr << "\n=== Reference Limitations ===" << std::endl;
        std::cerr << "a after ref = b: " << a << std::endl;
        std::cerr << "b: " << b << std::endl;
        std::cerr << "ref still refers to a: " << (&ref == &a) << std::endl;
        
        REQUIRE(a == 20);  // a got b's value
        REQUIRE(&ref == &a);  // ref still refers to a
        
        cpp_mastery::Logger::info("Reference limitations demonstrated");
    }
}

TEST_CASE("Pointer vs Reference Performance", "[performance][day2]") {
    const size_t array_size = 1000;
    
    // Test data
    auto data = cpp_mastery::generate_sequential_data<int>(array_size, 1);
    
    BENCHMARK("Pointer-based array traversal") {
        volatile long sum = 0;
        int* ptr = data.data();
        
        for (size_t j = 0; j < array_size; ++j) {
            sum += *(ptr + j);
        }
        
        return sum;
    };
    
    BENCHMARK("Reference-based array access") {
        volatile long sum = 0;
        
        for (size_t j = 0; j < array_size; ++j) {
            int& ref = data[j];
            sum += ref;
        }
        
        return sum;
    };
    
    BENCHMARK("Direct array indexing") {
        volatile long sum = 0;
        
        for (size_t j = 0; j < array_size; ++j) {
            sum += data[j];
        }
        
        return sum;
    };
    
    BENCHMARK("Your pointer arithmetic test") {
        volatile long sum = 0;
        int* start = data.data();
        int* end = start + data.size();
        for (int* ptr = start; ptr != end; ++ptr) {
            sum += *ptr;
        }        
        return sum;
    };
}

TEST_CASE("Function Parameter Passing", "[parameters][day2]") {
    
    // Helper functions for testing different parameter passing methods
    auto pass_by_value = [](int x) -> int {
        x = 999;  // Modifies local copy
        return x;
    };
    
    auto pass_by_pointer = [](int* x) -> int {
        if (x) {
            *x = 999;  // Modifies original through pointer
            return *x;
        }
        return 0;
    };
    
    auto pass_by_reference = [](int& x) -> int {
        x = 999;  // Modifies original through reference
        return x;
    };
    
    // Lambda capture examples
    int external_value = 42;
    int counter = 0;
    
    // Capture by value [=]
    auto capture_by_value = [=](int x) -> int {
        // external_value and counter are copied
        return x + external_value;  // Uses copy (42)
        // counter++;  // ERROR: can't modify copy
    };
    
    // Capture by reference [&]
    auto capture_by_reference = [&](int x) -> int {
        counter++;  // Modifies original counter
        return x + external_value + counter;
    };
    
    // Mixed capture [=, &counter]
    auto mixed_capture = [=, &counter](int x) -> int {
        counter++;  // Reference: modifies original
        return x + external_value;  // Value: uses copy
    };
    
    // Specific capture [external_value, &counter]
    auto specific_capture = [external_value, &counter](int x) -> int {
        counter++;
        return x + external_value;
        // Other variables not accessible
    };
    
    SECTION("Parameter passing comparison") {
        int original = 42;
        int copy1 = original, copy2 = original, copy3 = original;
        
        std::cerr << "\n=== Parameter Passing Methods ===" << std::endl;
        std::cerr << "Original values: " << copy1 << ", " << copy2 << ", " << copy3 << std::endl;
        
        // Test each method
        int result1 = pass_by_value(copy1);
        int result2 = pass_by_pointer(&copy2);
        int result3 = pass_by_reference(copy3);
        
        std::cerr << "After pass_by_value: copy1 = " << copy1 << ", result = " << result1 << std::endl;
        std::cerr << "After pass_by_pointer: copy2 = " << copy2 << ", result = " << result2 << std::endl;
        std::cerr << "After pass_by_reference: copy3 = " << copy3 << ", result = " << result3 << std::endl;
        
        // Verify behavior
        REQUIRE(copy1 == 42);   // Unchanged - passed by value
        REQUIRE(copy2 == 999);  // Modified - passed by pointer
        REQUIRE(copy3 == 999);  // Modified - passed by reference
        
        cpp_mastery::Logger::info("Parameter passing methods compared");
    }
    
    // TODO: Implement performance comparison of parameter passing methods
    SECTION("Parameter passing performance") {
        const size_t size = 1000;
        
        // TODO: Create benchmarks comparing:
        // 1. Pass large struct by value
        // 2. Pass large struct by const reference  
        // 3. Pass large struct by pointer
        // 
        struct LargeStruct {
            int data[size];
            LargeStruct() { std::fill(data, data + size, 42); }
        };

        // Helper functions for testing different parameter passing methods
        auto pass_by_value = [&](LargeStruct ls, int index) -> int {
            return ls.data[index];
        };
        
        auto pass_by_pointer = [](LargeStruct* ls, int index) -> int {
            if (ls)
                return ls->data[index];
        };
        
        auto pass_by_reference = [](LargeStruct& ls, int index) -> int {
            return ls.data[index];
        };

        LargeStruct myStruct;
        BENCHMARK("Pass by value") {
            int sum = 0;
            for (size_t i = 0; i < size; i++) {
                sum += pass_by_value(myStruct, static_cast<int>(i));
            }
            return sum;
        };
        BENCHMARK("Pass by pointer") {
            int sum = 0;
            for (size_t i = 0; i < size; i++) {
                sum += pass_by_pointer(&myStruct, static_cast<int>(i));
            }
            return sum;        };
        BENCHMARK("Pass by reference") {
            int sum = 0;
            for (size_t i = 0; i < size; i++) {
                sum += pass_by_reference(myStruct, static_cast<int>(i));
            }
            return sum;        };
        
        cpp_mastery::Logger::info("TODO: Implement parameter passing performance tests");
    }
}

TEST_CASE("Common Pointer Pitfalls", "[pitfalls][day2]") {
    
    SECTION("Dangling pointers") {
        int* dangling_ptr = nullptr;
        
        {
            int local_var = 42;
            dangling_ptr = &local_var;
            
            std::cerr << "\n=== Dangling Pointer Demo ===" << std::endl;
            std::cerr << "Inside scope - *dangling_ptr: " << *dangling_ptr << std::endl;
            
            REQUIRE(*dangling_ptr == 42);
        }
        // local_var is now destroyed, dangling_ptr is dangling!
        
        std::cerr << "Outside scope - dangling_ptr points to: " << dangling_ptr << std::endl;
        // DO NOT dereference dangling_ptr here - undefined behavior!
        
        cpp_mastery::Logger::info("Dangling pointer demonstrated (safely)");
    }
    
    SECTION("Memory leaks") {
        std::cerr << "\n=== Memory Leak Demo ===" << std::endl;
        
        // Allocate memory
        int* heap_ptr = new int(42);
        std::cerr << "Allocated memory at: " << heap_ptr << " with value: " << *heap_ptr << std::endl;
        
        REQUIRE(*heap_ptr == 42);
        
        // Proper cleanup
        delete heap_ptr;
        heap_ptr = nullptr;  // Good practice to avoid accidental reuse
        
        std::cerr << "Memory properly deallocated" << std::endl;
        
        cpp_mastery::Logger::info("Memory leak prevention demonstrated");
    }
    
    SECTION("Double delete and use after free") {
        int* ptr = new int(100);
        
        std::cerr << "\n=== Double Delete Prevention ===" << std::endl;
        std::cerr << "Allocated and using: " << *ptr << std::endl;
        
        REQUIRE(*ptr == 100);
        
        delete ptr;
        ptr = nullptr;  // Prevents double delete
        
        // This would be safe now (no-op)
        delete ptr;  // Deleting nullptr is safe
        
        cpp_mastery::Logger::info("Double delete prevention demonstrated");
    }
    
    // TODO: Implement buffer overflow demonstration
    SECTION("Buffer overflow prevention") {
        const size_t buffer_size = 10;
        int buffer[buffer_size];
        
        // TODO: Demonstrate safe buffer access
        // 1. Show how to safely iterate through buffer
        // 2. Demonstrate bounds checking
        // 3. Show what happens with out-of-bounds access (carefully!)
        //
        for (size_t i = 0; i < buffer_size; ++i) {
            buffer[i] = static_cast<int>(i);
        }
        //
        // Add your implementation here...
        
        cpp_mastery::Logger::info("TODO: Implement buffer overflow prevention demo");
    }
    
    SECTION("Memory copying methods") {
        const size_t buffer_size = 1000;
        const size_t iterations = 10000;
        
        // Source data
        auto source_data = cpp_mastery::generate_sequential_data<int>(buffer_size, 1);
        std::vector<int> dest_data(buffer_size);
        
        std::cerr << "\n=== Memory Copying Methods ===" << std::endl;
        
        BENCHMARK("std::memcpy") {
            for (size_t i = 0; i < iterations; ++i) {
                std::memcpy(dest_data.data(), source_data.data(), 
                           buffer_size * sizeof(int));
                cpp_mastery::do_not_optimize(dest_data.data());
            }
            return dest_data[0];
        };
        
        BENCHMARK("std::memmove") {
            for (size_t i = 0; i < iterations; ++i) {
                std::memmove(dest_data.data(), source_data.data(), 
                            buffer_size * sizeof(int));
                cpp_mastery::do_not_optimize(dest_data.data());
            }
            return dest_data[0];
        };
        
        BENCHMARK("std::copy") {
            for (size_t i = 0; i < iterations; ++i) {
                std::copy(source_data.begin(), source_data.end(), dest_data.begin());
                cpp_mastery::do_not_optimize(dest_data.data());
            }
            return dest_data[0];
        };
        
        BENCHMARK("Manual loop") {
            for (size_t i = 0; i < iterations; ++i) {
                for (size_t j = 0; j < buffer_size; ++j) {
                    dest_data[j] = source_data[j];
                }
                cpp_mastery::do_not_optimize(dest_data.data());
            }
            return dest_data[0];
        };
        
        // Verify all methods produce same result
        std::vector<int> test1(buffer_size), test2(buffer_size), test3(buffer_size);
        
        std::memcpy(test1.data(), source_data.data(), buffer_size * sizeof(int));
        std::copy(source_data.begin(), source_data.end(), test2.begin());
        for (size_t i = 0; i < buffer_size; ++i) {
            test3[i] = source_data[i];
        }
        
        REQUIRE(test1 == test2);
        REQUIRE(test2 == test3);
        
        cpp_mastery::Logger::info("Memory copying methods compared");
    }
}

TEST_CASE("Advanced Pointer Techniques", "[advanced][day2]") {
    
    SECTION("Pointer to pointer (double indirection)") {
        int value = 42;
        int* ptr = &value;
        int** ptr_to_ptr = &ptr;
        
        std::cerr << "\n=== Double Indirection ===" << std::endl;
        std::cerr << "value: " << value << std::endl;
        std::cerr << "*ptr: " << *ptr << std::endl;
        std::cerr << "**ptr_to_ptr: " << **ptr_to_ptr << std::endl;
        
        // Modify through double indirection
        **ptr_to_ptr = 100;
        
        REQUIRE(value == 100);
        REQUIRE(*ptr == 100);
        REQUIRE(**ptr_to_ptr == 100);
        
        cpp_mastery::Logger::info("Double indirection verified");
    }
    
    SECTION("Function pointers") {
        // Function pointer declarations
        int (*add_func)(int, int) = [](int a, int b) -> int { return a + b; };
        int (*mult_func)(int, int) = [](int a, int b) -> int { return a * b; };
        
        std::cerr << "\n=== Function Pointers ===" << std::endl;
        
        // Use function pointers
        int result1 = add_func(5, 3);
        int result2 = mult_func(5, 3);
        
        std::cerr << "add_func(5, 3) = " << result1 << std::endl;
        std::cerr << "mult_func(5, 3) = " << result2 << std::endl;
        
        REQUIRE(result1 == 8);
        REQUIRE(result2 == 15);
        
        // Array of function pointers
        int (*operations[])(int, int) = {add_func, mult_func};
        
        for (size_t i = 0; i < 2; ++i) {
            int result = operations[i](10, 2);
            std::cerr << "operations[" << i << "](10, 2) = " << result << std::endl;
        }
        
        cpp_mastery::Logger::info("Function pointers demonstrated");
    }
    
    SECTION("Function pointer call mechanics") {
        // Direct function for comparison
        auto direct_add = [](int a, int b) -> int { return a + b; };
        
        // Function pointer version
        int (*ptr_add)(int, int) = [](int a, int b) -> int { return a + b; };
        
        std::cerr << "\n=== Function Call Mechanics ===" << std::endl;
        std::cerr << "Direct lambda address: " << reinterpret_cast<void*>(&direct_add) << std::endl;
        std::cerr << "Function pointer value: " << reinterpret_cast<void*>(ptr_add) << std::endl;
        std::cerr << "Function pointer address: " << reinterpret_cast<void*>(&ptr_add) << std::endl;
        
        // Performance comparison
        const size_t iterations = 1000000;
        volatile int sum = 0;
        
        BENCHMARK("No function calls") {
            for (size_t i = 0; i < iterations; ++i) {
                sum += static_cast<int>(i) + 1;  // Direct call
            }
            return sum;
        };

        BENCHMARK("Direct function calls") {
            for (size_t i = 0; i < iterations; ++i) {
                sum += direct_add(static_cast<int>(i), 1);  // Direct call
            }
            return sum;
        };
        
        BENCHMARK("Function pointer calls") {
            for (size_t i = 0; i < iterations; ++i) {
                sum += ptr_add(static_cast<int>(i), 1);  // Indirect call
            }
            return sum;
        };
        
        // Array of function pointers (even more indirection)
        int (*func_array[])(int, int) = {ptr_add, ptr_add, ptr_add};
        
        BENCHMARK("Array of function pointers") {
            for (size_t i = 0; i < iterations; ++i) {
                size_t idx = i % 3;
                sum += func_array[idx](static_cast<int>(i), 1);  // Double indirection
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Function pointer call mechanics analyzed");
    }
    
    // TODO: Implement pointer arithmetic challenges
    SECTION("Pointer arithmetic challenges") {
        // TODO: Implement these challenges:
        
        // Challenge 1: Reverse an array using only pointer arithmetic
        int arr[] = {1, 2, 3, 4, 5};
        size_t size = sizeof(arr) / sizeof(arr[0]);
        
        // TODO: Implement array reversal using pointers
        // int* start = arr;
        // int* end = arr + size - 1;
        // while (start < end) {
        //     // Swap *start and *end
        //     // Move pointers toward each other
        // }
        
        // Challenge 2: Find element in array using pointer arithmetic
        int target = 3;
        // TODO: Implement linear search using only pointers
        // int* found = nullptr;
        // for (int* p = arr; p < arr + size; ++p) {
        //     if (*p == target) {
        //         found = p;
        //         break;
        //     }
        // }
        
        cpp_mastery::Logger::info("TODO: Implement pointer arithmetic challenges");
    }
    
    SECTION("Vector pointer bounds checking") {
        std::vector<int> vec = {10, 20, 30, 40, 50};
        
        std::cerr << "\n=== Vector Pointer Bounds ===" << std::endl;
        std::cerr << "Vector size: " << vec.size() << std::endl;
        std::cerr << "Vector data(): " << vec.data() << std::endl;
        std::cerr << "Vector end (data() + size()): " << (vec.data() + vec.size()) << std::endl;
        
        // Safe iteration using pointer bounds
        int* ptr = vec.data();
        int* end_ptr = vec.data() + vec.size();
        
        std::cerr << "Iterating with pointer bounds check:" << std::endl;
        while (ptr != end_ptr) {
            std::cerr << "  *ptr = " << *ptr << " at address " << ptr << std::endl;
            ++ptr;
        }
        
        // Verify ptr is now at end
        REQUIRE(ptr == vec.data() + vec.size());
        
        // Helper functions for bounds checking
        auto is_at_end = [&](int* p) { return p == (vec.data() + vec.size()); };
        auto is_in_bounds = [&](int* p) { return p >= vec.data() && p < (vec.data() + vec.size()); };
        
        REQUIRE(is_at_end(ptr));
        REQUIRE(!is_in_bounds(ptr));  // At end means not in bounds for dereferencing
        
        // Reset to beginning
        ptr = vec.data();
        REQUIRE(!is_at_end(ptr));
        REQUIRE(is_in_bounds(ptr));
        
        cpp_mastery::Logger::info("Vector pointer bounds checking verified");
    }
}

TEST_CASE("Memory Address Analysis", "[memory][day2]") {
    
    SECTION("Stack vs heap addresses") {
        // Stack variables
        int stack_var1 = 1;
        int stack_var2 = 2;
        int stack_array[10];
        
        // Heap variables
        int* heap_var1 = new int(1);
        int* heap_var2 = new int(2);
        int* heap_array = new int[10];
        
        std::cerr << "\n=== Stack vs Heap Addresses ===" << std::endl;
        std::cerr << "Stack variables:" << std::endl;
        std::cerr << "  stack_var1: " << &stack_var1 << std::endl;
        std::cerr << "  stack_var2: " << &stack_var2 << std::endl;
        std::cerr << "  stack_array: " << stack_array << std::endl;
        
        std::cerr << "Heap variables:" << std::endl;
        std::cerr << "  heap_var1: " << heap_var1 << std::endl;
        std::cerr << "  heap_var2: " << heap_var2 << std::endl;
        std::cerr << "  heap_array: " << heap_array << std::endl;
        
        // Analyze address patterns
        std::cerr << "Stack variable distance: " 
                 << reinterpret_cast<char*>(&stack_var2) - reinterpret_cast<char*>(&stack_var1) 
                 << " bytes" << std::endl;
        
        std::cerr << "Heap variable distance: " 
                 << reinterpret_cast<char*>(heap_var2) - reinterpret_cast<char*>(heap_var1) 
                 << " bytes" << std::endl;
        
        // TODO: Analyze and document the patterns you observe
        // Questions to investigate:
        // 1. Are stack addresses higher or lower than heap addresses?
        // 2. Do stack variables have predictable spacing?
        // 3. Are heap allocations contiguous?
        // 4. What happens with multiple allocations?
        
        // Cleanup
        delete heap_var1;
        delete heap_var2;
        delete[] heap_array;
        
        cpp_mastery::Logger::info("Memory address analysis completed");
    }
    
    // TODO: Implement pointer size analysis
    SECTION("Pointer size analysis") {
        // TODO: Investigate pointer sizes for different types
        // 
        std::cerr << "sizeof(char*): " << sizeof(char*) << std::endl;
        std::cerr << "sizeof(int*): " << sizeof(int*) << std::endl;
        std::cerr << "sizeof(double*): " << sizeof(double*) << std::endl;
        std::cerr << "sizeof(void*): " << sizeof(void*) << std::endl;
        //
        // Questions:
        // 1. Are all pointer types the same size?
        // 2. How does pointer size relate to architecture (32-bit vs 64-bit)?
        // 3. What about function pointers?
        
        cpp_mastery::Logger::info("TODO: Implement pointer size analysis");
    }
}

// =============================================================================
// EXERCISES FOR COMPLETION
// =============================================================================

TEST_CASE("Day 2 Exercises - Complete These!", "[exercises][day2]") {
    
    SECTION("Exercise 1: Implement pointer-based string length") {
        const char* test_string = "Hello, World!";
        
        // TODO: Implement strlen using only pointer arithmetic
        // Do not use array indexing or std::strlen
        auto my_strlen = [](const char* str) -> size_t {
            size_t idx = 0;
            while (*(str + idx) != '\0') {
                idx++;
            }
            return idx;  // Replace with your implementation
        };
        
        size_t length = my_strlen(test_string);
        std::cerr << "String: \"" << test_string << "\" has length: " << length << std::endl;
        
        REQUIRE(length == 13);  // Uncomment when implemented
        
        cpp_mastery::Logger::info("TODO: Complete string length implementation");
    }
    
    SECTION("Exercise 2: Swap function implementations") {
        int a = 10, b = 20;
        int original_a = a, original_b = b;
        
        // TODO: Implement swap using pointers
        auto swap_with_pointers = [](int* x, int* y) {
            int* tmp = x;
            x = y;
            y = tmp;
        };
        
        // TODO: Implement swap using references  
        auto swap_with_references = [](int& x, int& y) {
            int tmp = x;
            x = y;
            y = tmp;
        };
        
        std::cerr << "Before swap: a = " << a << ", b = " << b << std::endl;
        
        // Test pointer version
        swap_with_pointers(&a, &b);
        std::cerr << "After pointer swap: a = " << a << ", b = " << b << std::endl;
        
        // Reset and test reference version
        a = original_a; b = original_b;
        swap_with_references(a, b);
        std::cerr << "After reference swap: a = " << a << ", b = " << b << std::endl;
        
        REQUIRE(a == original_b);  // Uncomment when implemented
        REQUIRE(b == original_a);  // Uncomment when implemented
        
        cpp_mastery::Logger::info("TODO: Complete swap implementations");
    }
    
    // TODO: Implement a simple dynamic array class using raw pointers
    // Requirements:
    // 1. Constructor that allocates memory
    // 2. Destructor that frees memory
    // 3. Copy constructor (deep copy)
    // 4. Assignment operator (deep copy)
    // 5. Access operators ([] and at())
    // 6. Size and capacity functions
    class DynamicArray {
        private:
            int* data_;
            size_t size_;
            size_t capacity_;
            
        public:
            explicit DynamicArray(size_t size, size_t capacity) : data_(nullptr), size_(size), capacity_(capacity) {
                if (capacity > 0)
                    data_ = static_cast<int*>(malloc(sizeof(int)*capacity));
            }
            
            ~DynamicArray() {
                free(data_);
            }
            
            DynamicArray(const DynamicArray& other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
                if (capacity_ > 0) {
                    data_ = static_cast<int*>(malloc(sizeof(int)*capacity_));
                    std::memcpy(data_, other.data_, size_*sizeof(int));
                }
            }
            
            DynamicArray& operator=(const DynamicArray& other) {
                if (data_ == other.data_)
                    return *this;
                size_ = other.size_;
                if (capacity_ < size_) {
                    free(data_);
                    data_ = static_cast<int*>(malloc(sizeof(int)*other.capacity_));
                    capacity_ = other.capacity_;
                }
                std::memcpy(data_, other.data_, other.size_*sizeof(int));
                return *this;
            }

            void pop() {
                if (size_ > 0)
                    size_--;
            }

            void push(int val) {
                if (size_ == capacity_) {
                    capacity_ = 2 * (size_ + 1);
                    int* old_data = data_;
                    data_ = static_cast<int*>(malloc(sizeof(int)*capacity_));
                    std::memcpy(data_, old_data, size_*sizeof(int));
                    free(old_data);
                }
                *(data_ + size_) = val;
                size_++;
            }
            
            int& operator[](size_t index) {
                return *(data_ + index);
            }
            
            size_t size() const { return size_; }
            size_t capacity() const { return capacity_; }
    };

    SECTION("Exercise 3: Dynamic array management") {        
        DynamicArray da(0, 0);
        REQUIRE(da.size() == 0);
        REQUIRE(da.capacity() == 0);
        da.pop();
        REQUIRE(da.size() == 0);
        for (int i = 0; i < 100; i++) {
            da.push(i);
            REQUIRE(da.size() == (i+1));
            REQUIRE(da[i] == i);
        }
        for (int i = 0; i < 100; i++) {
            REQUIRE(da[i] == i);
        }
        REQUIRE(da.capacity() == 126);

        DynamicArray da_cpy = da;
        for (int i = 0; i < 100; i++) {
            REQUIRE(da_cpy[i] == da[i]);
        }
        da_cpy.push(100);
        da_cpy[99] = -77;
        da_cpy = da;
        REQUIRE(da_cpy[99] == 99);
        REQUIRE(da_cpy.size() == da.size());
        da_cpy[99] = -77;
        da_cpy.push(100);
        REQUIRE(da_cpy.size() == da.size() + 1);
        da = da_cpy;
        REQUIRE(da[100] == 100);
    }

    BENCHMARK("std::vector push") {
        std::vector<int> myVec;
        for (int i = 0; i < 100; i++) {
            myVec.push_back(i);
        }
        return myVec[99];
    };

    BENCHMARK("std::vector push w/ reserve") {
        std::vector<int> myVec;
        myVec.reserve(100);
        for (int i = 0; i < 100; i++) {
            myVec.push_back(i);
        }
        return myVec[99];
    };

    BENCHMARK("DynamicArray push") {
        DynamicArray myArr(0, 100);
        for (int i = 0; i < 100; i++) {
            myArr.push(i);
        }
        return myArr[99];
    };
}