#include "cpp_mastery.hpp"
#include <functional>
#include <vector>
#include <algorithm>

// =============================================================================
// DAY 4: Functions and Parameter Passing
// =============================================================================
// Topics Covered:
// - Pass by value vs reference vs pointer
// - Function overloading resolution
// - Inline functions and performance
// - Function pointers and callbacks
// =============================================================================

// Forward declarations for testing
void demonstrate_pass_by_value(int x);
void demonstrate_pass_by_reference(int& x);
void demonstrate_pass_by_pointer(int* x);
typedef int MouseEvent;

TEST_CASE("Parameter Passing Methods", "[functions][parameters][day4]") {
    
    SECTION("Pass by value vs reference vs pointer") {
        int original = 42;
        int test_value = original;
        int test_reference = original;
        int test_pointer = original;
        
        std::cerr << "\n=== Parameter Passing Comparison ===" << std::endl;
        std::cerr << "Original values: " << test_value << ", " << test_reference << ", " << test_pointer << std::endl;
        
        // Pass by value - creates copy
        demonstrate_pass_by_value(test_value);
        std::cerr << "After pass by value: " << test_value << " (unchanged)" << std::endl;
        
        // Pass by reference - modifies original
        demonstrate_pass_by_reference(test_reference);
        std::cerr << "After pass by reference: " << test_reference << " (modified)" << std::endl;
        
        // Pass by pointer - modifies original through pointer
        demonstrate_pass_by_pointer(&test_pointer);
        std::cerr << "After pass by pointer: " << test_pointer << " (modified)" << std::endl;
        
        // Verify behavior
        REQUIRE(test_value == original);      // Unchanged
        REQUIRE(test_reference != original);  // Modified
        REQUIRE(test_pointer != original);    // Modified
        
        cpp_mastery::Logger::info("Parameter passing methods compared");
    }
    
    SECTION("Large object parameter passing performance") {
        struct LargeObject {
            int data[1000];
            LargeObject() { std::fill(data, data + 1000, 42); }
            int sum() const {
                int total = 0;
                for (int i = 0; i < 1000; ++i) {
                    total += data[i];
                }
                return total;
            }
        };
        
        LargeObject large_obj;
        const size_t iterations = 10000;
        
        // Function that takes by value (expensive copy)
        auto process_by_value = [](LargeObject obj) -> int {
            return obj.sum();
        };
        
        // Function that takes by const reference (no copy)
        auto process_by_const_ref = [](const LargeObject& obj) -> int {
            return obj.sum();
        };
        
        // Function that takes by pointer (no copy)
        auto process_by_pointer = [](const LargeObject* obj) -> int {
            return obj->sum();
        };
        
        BENCHMARK("Pass large object by value") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += process_by_value(large_obj);  // Expensive copy each time
            }
            return sum;
        };
        
        BENCHMARK("Pass large object by const reference") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += process_by_const_ref(large_obj);  // No copy
            }
            return sum;
        };
        
        BENCHMARK("Pass large object by pointer") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += process_by_pointer(&large_obj);  // No copy
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Large object parameter passing performance measured");
    }
    
    SECTION("Array parameter passing") {
        int static_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Function that takes array (actually receives pointer)
        auto process_array = [](int arr[], size_t size) {
            std::cerr << "Inside function - sizeof(arr): " << sizeof(arr) << " bytes" << std::endl;
            for (size_t i = 0; i < size; ++i) {
                arr[i] *= 2;
            }
        };
        
        // Function that takes array by reference (preserves array type)
        auto process_array_ref = [](int (&arr)[10]) -> int {
            std::cerr << "Inside function - sizeof(arr): " << sizeof(arr) << " bytes" << std::endl;
            for (size_t i = 0; i < 10; ++i) {
                arr[i] *= 2;
            }
        };
        
        std::cerr << "\n=== Array Parameter Passing ===" << std::endl;
        std::cerr << "Original array sizeof: " << sizeof(static_array) << " bytes" << std::endl;
        
        for (int i = 0; i < 10; i++) {
            REQUIRE(static_array[i] == i+1);
        }
        process_array(static_array, 10);  // Array decays to pointer
        for (int i = 0; i < 10; i++) {
            REQUIRE(static_array[i] == 2*(i+1));
        }
        process_array_ref(static_array);  // Array reference preserves type
        for (int i = 0; i < 10; i++) {
            REQUIRE(static_array[i] == 4*(i+1));
        }
        
        cpp_mastery::Logger::info("Array parameter passing demonstrated");
    }
}

// Implementation of parameter passing demonstration functions
void demonstrate_pass_by_value(int x) {
    x = 999;  // Modifies local copy only
    std::cerr << "Inside pass_by_value: x = " << x << std::endl;
}

void demonstrate_pass_by_reference(int& x) {
    x = 888;  // Modifies original through reference
    std::cerr << "Inside pass_by_reference: x = " << x << std::endl;
}

void demonstrate_pass_by_pointer(int* x) {
    if (x) {
        *x = 777;  // Modifies original through pointer
        std::cerr << "Inside pass_by_pointer: *x = " << *x << std::endl;
    }
}

// Example structure:
void test_func(int x) {
    std::cout << "Int overload of test_func" << '\n';
};
void test_func(double x) {
    std::cout << "double overload of test_func" << '\n';
};
void test_func(const std::string& x) {
    std::cout << "std::string overload of test_func" << '\n';
};
void test_func(char x) {
    std::cout << "char overload of test_func" << '\n';
};

// Function template approach
template<typename T>
auto template_func = [](const T& value) -> std::string {
    return "Template: " + std::to_string(value);
};

TEST_CASE("Function Overloading", "[functions][overloading][day4]") {
    
    SECTION("Basic function overloading") {
        // Overloaded functions with different parameter types
        auto print_value = [](int x) -> std::string {
            return "int: " + std::to_string(x);
        };
        
        auto print_value_double = [](double x) -> std::string {
            return "double: " + std::to_string(x);
        };
        
        auto print_value_string = [](const std::string& x) -> std::string {
            return "string: " + x;
        };
        
        std::cerr << "\n=== Function Overloading ===" << std::endl;
        
        // Note: Lambdas can't be overloaded, so we simulate with different names
        // In real code, these would all be named the same
        std::cerr << print_value(42) << std::endl;
        std::cerr << print_value_double(3.14) << std::endl;
        std::cerr << print_value_string("Hello") << std::endl;
        
        cpp_mastery::Logger::info("Function overloading demonstrated with lambdas");
    }
    
    // TODO: Implement actual overloaded functions
    SECTION("Overload resolution rules") {
        // TODO: Create a set of overloaded functions that demonstrate:
        // 1. Exact match
        // 2. Promotion (char -> int, float -> double)
        // 3. Standard conversion (int -> double, pointer -> bool)
        // 4. User-defined conversion
        // 5. Ambiguous calls
        
        // Test calls:
        test_func(42);        // Exact match to int
        test_func(3.14);      // Exact match to double
        test_func('A');       // Exact match to char
        test_func("hello");   // Conversion to string
        
        cpp_mastery::Logger::info("TODO: Implement overload resolution examples");
    }
    
    SECTION("Template vs overloading") {
        
        // TODO: Compare template approach vs function overloading
        // 1. When to use templates vs overloads
        // 2. Performance implications
        // 3. Code generation differences
        // 4. Type safety considerations
        template_func<int>(42);        // Exact match to int
        template_func<double>(3.14);      // Exact match to double
        template_func<char>('A');       // Exact match to char
        // template_func<std::string>("hello");   // Conversion to string
    }
}

// Regular function (not inline)
int regular_add(int a, int b) {
    return a + b;
};

inline auto inline_add = [](int a, int b) -> int {
    return a + b;
};

TEST_CASE("Inline Functions and Performance", "[functions][inline][performance][day4]") {
    
    SECTION("Inline function demonstration") {
        
        const size_t iterations = 100000;
        
        BENCHMARK("Regular function calls") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += regular_add(static_cast<int>(i), 1);
            }
            return sum;
        };
        
        BENCHMARK("Inline function calls") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += inline_add(static_cast<int>(i), 1);
            }
            return sum;
        };
        
        BENCHMARK("Direct operation (baseline)") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += static_cast<int>(i) + 1;  // Direct operation
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Inline function performance measured");
    }
}

TEST_CASE("Function Pointers and Callbacks", "[functions][pointers][callbacks][day4]") {
    
    SECTION("Basic function pointers") {
        // Function pointer declarations and usage
        int (*operation)(int, int) = nullptr;
        
        // Functions to point to
        auto add_func = [](int a, int b) -> int { return a + b; };
        auto multiply_func = [](int a, int b) -> int { return a * b; };
        auto subtract_func = [](int a, int b) -> int { return a - b; };
        
        std::cerr << "\n=== Function Pointers ===" << std::endl;
        
        // Array of function pointers
        int (*operations[])(int, int) = {
            [](int a, int b) -> int { return a + b; },  // Add
            [](int a, int b) -> int { return a - b; },  // Subtract
            [](int a, int b) -> int { return a * b; },  // Multiply
            [](int a, int b) -> int { return a / b; }   // Divide
        };
        
        const char* op_names[] = {"Add", "Subtract", "Multiply", "Divide"};
        
        for (int i = 0; i < 4; ++i) {
            int result = operations[i](10, 2);
            std::cerr << op_names[i] << "(10, 2) = " << result << std::endl;
        }
        
        cpp_mastery::Logger::info("Function pointers demonstrated");
    }
    
    SECTION("Callback functions") {
        // Function that takes a callback
        auto process_array = [](const std::vector<int>& arr, 
                               std::function<void(int)> callback) -> void {
            for (int value : arr) {
                callback(value);
            }
        };
        
        std::vector<int> data = {1, 2, 3, 4, 5};
        
        std::cerr << "\n=== Callback Functions ===" << std::endl;
        
        // Different callback functions
        auto print_callback = [](int x) {
            std::cerr << "Value: " << x << "\n";
        };
        
        auto square_callback = [](int x) {
            std::cerr << "Square: " << (x * x) << "\n";
        };
        
        std::cerr << "Print callback: ";
        process_array(data, print_callback);
        std::cerr << std::endl;
        
        std::cerr << "Square callback: ";
        process_array(data, square_callback);
        std::cerr << std::endl;
        
        cpp_mastery::Logger::info("Callback functions demonstrated");
    }
    
    SECTION("Function pointer performance") {
        const size_t iterations = 1000000;
        
        // Function pointer call
        int (*ptr_add)(int, int) = [](int a, int b) -> int { return a + b; };
        
        // std::function call
        std::function<int(int, int)> func_add = [](int a, int b) -> int { return a + b; };
        
                // Direct function call
        auto direct_add = [](int a, int b) -> int { return a + b; };
        
        BENCHMARK("Function pointer call") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += ptr_add(static_cast<int>(i), 1);
            }
            return sum;
        };
        
        BENCHMARK("std::function call") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += func_add(static_cast<int>(i), 1);
            }
            return sum;
        };

        BENCHMARK("Direct function call") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += direct_add(static_cast<int>(i), 1);
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Function pointer performance compared");
    }
}

TEST_CASE("Function Return Types and Values", "[functions][return][day4]") {
    
    SECTION("Returning local variables (dangling references)") {
        // DANGEROUS: Returning reference to local variable
        auto dangerous_function = []() -> int& {
            int local_var = 42;
            // return local_var;  // DON'T DO THIS - dangling reference!
            static int safe_static = 42;
            return safe_static;  // This is safe - static storage
        };
        
        // SAFE: Returning copy of local variable
        auto safe_function = []() -> int {
            int local_var = 42;
            return local_var;  // Safe - returns copy
        };
        
        std::cerr << "\n=== Returning Local Variables ===" << std::endl;
        
        int& ref = dangerous_function();  // Safe because we return static
        int copy = safe_function();       // Safe - copy of local
        
        std::cerr << "Reference to static: " << ref << std::endl;
        std::cerr << "Copy of local: " << copy << std::endl;
        
        cpp_mastery::Logger::info("Local variable return safety demonstrated");
    }
}


// =============================================================================
// EXERCISES FOR COMPLETION
// =============================================================================

TEST_CASE("Day 4 Exercises - Complete These!", "[exercises][day4]") {
    
    SECTION("Exercise 1: Function overloading calculator") {
        // TODO: Implement a calculator with overloaded functions
        // Requirements:
        // 1. Overload calculate() for different parameter types
        // 2. Support int, double, and complex operations
        // 3. Handle different numbers of parameters (2, 3, 4 operands)
        // 4. Demonstrate overload resolution rules
        
        // Example structure:
        // int calculate(int a, int b);
        // double calculate(double a, double b);
        // int calculate(int a, int b, int c);
        // std::string calculate(const std::string& op, int a, int b);
        
        cpp_mastery::Logger::info("TODO: Implement overloaded calculator");
    }
    
    SECTION("Exercise 2: Callback-based event system") {
        // TODO: Implement an event system using callbacks
        // Requirements:
        // 1. Event manager that can register multiple callbacks
        // 2. Different event types (mouse, keyboard, timer)
        // 3. Callback functions with different signatures
        // 4. Performance comparison: function pointers vs std::function
        // Example structure:
        class EventSystem {
            std::vector<std::function<void(MouseEvent)>> mouse_callbacks;
        public:
            void register_mouse_callback(std::function<void(MouseEvent)> cb) {
                mouse_callbacks.push_back(cb);
            }
            void trigger_mouse_event(MouseEvent event) {
                for (auto cb : mouse_callbacks) {
                    cb(event);
                }
            }
        };

        EventSystem es;
        es.register_mouse_callback([](MouseEvent event) -> void {
            std::cout << "Callback #1, logging event: " << event << '\n';
        });
        es.register_mouse_callback([](MouseEvent event) -> void {
            std::cout << "Callback #2, logging event: " << event << '\n';
        });
        es.trigger_mouse_event(2);
        es.trigger_mouse_event(99);
    }
}