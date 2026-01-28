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
        auto process_array = [](int arr[], size_t size) -> int {
            std::cerr << "Inside function - sizeof(arr): " << sizeof(arr) << " bytes" << std::endl;
            int sum = 0;
            for (size_t i = 0; i < size; ++i) {
                sum += arr[i];
            }
            return sum;
        };
        
        // Function that takes array by reference (preserves array type)
        auto process_array_ref = [](int (&arr)[10]) -> int {
            std::cerr << "Inside function - sizeof(arr): " << sizeof(arr) << " bytes" << std::endl;
            int sum = 0;
            for (size_t i = 0; i < 10; ++i) {
                sum += arr[i];
            }
            return sum;
        };
        
        std::cerr << "\n=== Array Parameter Passing ===" << std::endl;
        std::cerr << "Original array sizeof: " << sizeof(static_array) << " bytes" << std::endl;
        
        int sum1 = process_array(static_array, 10);  // Array decays to pointer
        int sum2 = process_array_ref(static_array);  // Array reference preserves type
        
        std::cerr << "Sum from pointer version: " << sum1 << std::endl;
        std::cerr << "Sum from reference version: " << sum2 << std::endl;
        
        REQUIRE(sum1 == sum2);  // Both should calculate same sum
        REQUIRE(sum1 == 55);    // 1+2+...+10 = 55
        
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
        
        // Example structure:
        // void test_func(int x);
        // void test_func(double x);
        // void test_func(const std::string& x);
        // void test_func(char x);
        
        // Test calls:
        // test_func(42);        // Exact match to int
        // test_func(3.14);      // Exact match to double
        // test_func('A');       // Exact match to char
        // test_func("hello");   // Conversion to string
        
        cpp_mastery::Logger::info("TODO: Implement overload resolution examples");
    }
    
    SECTION("Template vs overloading") {
        // Function template approach
        template<typename T>
        auto template_func = [](T value) -> std::string {
            return "Template: " + std::to_string(value);
        };
        
        // TODO: Compare template approach vs function overloading
        // 1. When to use templates vs overloads
        // 2. Performance implications
        // 3. Code generation differences
        // 4. Type safety considerations
        
        cpp_mastery::Logger::info("TODO: Compare templates vs overloading");
    }
}

TEST_CASE("Inline Functions and Performance", "[functions][inline][performance][day4]") {
    
    SECTION("Inline function demonstration") {
        // Regular function (not inline)
        auto regular_add = [](int a, int b) -> int {
            return a + b;
        };
        
        // Inline function (compiler hint)
        // Note: lambdas are typically inlined automatically
        inline auto inline_add = [](int a, int b) -> int {
            return a + b;
        };
        
        const size_t iterations = 10000000;
        
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
    
    // TODO: Implement inline function analysis
    SECTION("When to use inline functions") {
        // TODO: Demonstrate scenarios where inline helps vs hurts:
        // 1. Small, frequently called functions (good for inline)
        // 2. Large functions (bad for inline - code bloat)
        // 3. Recursive functions (usually not inlined)
        // 4. Virtual functions (cannot be inlined at call site)
        // 5. Functions with loops (may not benefit from inlining)
        
        // TODO: Show assembly output differences between inlined and non-inlined
        
        cpp_mastery::Logger::info("TODO: Analyze when to use inline functions");
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
            std::cerr << "Value: " << x << " ";
        };
        
        auto square_callback = [](int x) {
            std::cerr << "Square: " << (x * x) << " ";
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
        
        // Direct function call
        auto direct_add = [](int a, int b) -> int { return a + b; };
        
        // Function pointer call
        int (*ptr_add)(int, int) = [](int a, int b) -> int { return a + b; };
        
        // std::function call
        std::function<int(int, int)> func_add = [](int a, int b) -> int { return a + b; };
        
        BENCHMARK("Direct function call") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                sum += direct_add(static_cast<int>(i), 1);
            }
            return sum;
        };
        
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
        
        cpp_mastery::Logger::info("Function pointer performance compared");
    }
    
    // TODO: Implement advanced callback patterns
    SECTION("Advanced callback patterns") {
        // TODO: Implement examples of:
        // 1. Event handling with callbacks
        // 2. Strategy pattern with function pointers
        // 3. Observer pattern with callbacks
        // 4. Functional programming patterns (map, filter, reduce)
        
        // Example structure:
        // class EventManager {
        //     std::vector<std::function<void(int)>> callbacks;
        // public:
        //     void register_callback(std::function<void(int)> cb);
        //     void trigger_event(int data);
        // };
        
        cpp_mastery::Logger::info("TODO: Implement advanced callback patterns");
    }
}

TEST_CASE("Function Return Types and Values", "[functions][return][day4]") {
    
    SECTION("Return by value vs reference") {
        std::vector<int> global_data = {1, 2, 3, 4, 5};
        
        // Return by value (copy)
        auto get_copy = []() -> std::vector<int> {
            return global_data;  // Returns copy
        };
        
        // Return by reference (alias)
        auto get_reference = []() -> std::vector<int>& {
            return global_data;  // Returns reference to original
        };
        
        // Return by const reference (read-only alias)
        auto get_const_reference = []() -> const std::vector<int>& {
            return global_data;  // Returns const reference
        };
        
        std::cerr << "\n=== Return Types ===" << std::endl;
        
        // Test return by copy
        auto copy = get_copy();
        copy[0] = 999;  // Modify copy
        std::cerr << "Original after modifying copy: " << global_data[0] << std::endl;
        
        // Test return by reference
        auto& ref = get_reference();
        ref[0] = 888;  // Modify original through reference
        std::cerr << "Original after modifying reference: " << global_data[0] << std::endl;
        
        // Test return by const reference
        const auto& const_ref = get_const_reference();
        // const_ref[0] = 777;  // Would not compile - const reference
        std::cerr << "Value through const reference: " << const_ref[0] << std::endl;
        
        REQUIRE(global_data[0] == 888);  // Modified by reference
        REQUIRE(copy[0] == 999);         // Copy was modified independently
        
        cpp_mastery::Logger::info("Return types demonstrated");
    }
    
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
    
    // TODO: Implement return value optimization examples
    SECTION("Return Value Optimization (RVO)") {
        // TODO: Demonstrate:
        // 1. Named Return Value Optimization (NRVO)
        // 2. Copy elision
        // 3. Move semantics with returns
        // 4. When RVO applies and when it doesn't
        
        // Example:
        // std::vector<int> create_large_vector() {
        //     std::vector<int> result(1000000, 42);  // Large object
        //     return result;  // RVO should eliminate copy
        // }
        
        cpp_mastery::Logger::info("TODO: Implement RVO examples");
    }
}

TEST_CASE("Function Templates and Generic Programming", "[functions][templates][day4]") {
    
    SECTION("Basic function templates") {
        // Template function for finding maximum
        template<typename T>
        T find_max(T a, T b) {
            return (a > b) ? a : b;
        }
        
        std::cerr << "\n=== Function Templates ===" << std::endl;
        
        // Template instantiation with different types
        int max_int = find_max(10, 20);
        double max_double = find_max(3.14, 2.71);
        std::string max_string = find_max(std::string("apple"), std::string("banana"));
        
        std::cerr << "Max int: " << max_int << std::endl;
        std::cerr << "Max double: " << max_double << std::endl;
        std::cerr << "Max string: " << max_string << std::endl;
        
        REQUIRE(max_int == 20);
        REQUIRE(max_double == 3.14);
        REQUIRE(max_string == "banana");
        
        cpp_mastery::Logger::info("Function templates demonstrated");
    }
    
    // TODO: Implement advanced template features
    SECTION("Template specialization and constraints") {
        // TODO: Implement:
        // 1. Explicit template specialization
        // 2. Template argument deduction
        // 3. SFINAE (Substitution Failure Is Not An Error)
        // 4. Template constraints (C++20 concepts preview)
        
        // Example specialization:
        // template<typename T>
        // void print_type(T value) { std::cout << "Generic: " << value; }
        // 
        // template<>
        // void print_type<const char*>(const char* value) { 
        //     std::cout << "String: " << value; 
        // }
        
        cpp_mastery::Logger::info("TODO: Implement template specialization");
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
        // class EventSystem {
        //     std::vector<std::function<void(MouseEvent)>> mouse_callbacks;
        //     std::vector<std::function<void(KeyEvent)>> key_callbacks;
        // public:
        //     void register_mouse_callback(std::function<void(MouseEvent)> cb);
        //     void trigger_mouse_event(MouseEvent event);
        // };
        
        cpp_mastery::Logger::info("TODO: Implement callback-based event system");
    }
    
    SECTION("Exercise 3: Generic algorithm library") {
        // TODO: Implement generic algorithms using function templates
        // Requirements:
        // 1. Template functions for common algorithms (sort, search, transform)
        // 2. Support for different container types
        // 3. Customizable comparison and transformation functions
        // 4. Performance benchmarks against STL equivalents
        
        // Example functions to implement:
        // template<typename Iterator, typename Compare>
        // void my_sort(Iterator begin, Iterator end, Compare comp);
        // 
        // template<typename Container, typename Predicate>
        // auto my_filter(const Container& c, Predicate pred);
        // 
        // template<typename Container, typename Transform>
        // auto my_map(const Container& c, Transform func);
        
        cpp_mastery::Logger::info("TODO: Implement generic algorithm library");
    }
    
    SECTION("Exercise 4: Function performance analyzer") {
        // TODO: Create a system to analyze function call performance
        // Requirements:
        // 1. Measure function call overhead for different call types
        // 2. Compare inline vs non-inline functions
        // 3. Analyze parameter passing costs (value, reference, pointer)
        // 4. Create performance reports with recommendations
        
        // Example structure:
        // class FunctionProfiler {
        //     std::map<std::string, std::vector<double>> timings;
        // public:
        //     template<typename Func, typename... Args>
        //     auto profile(const std::string& name, Func f, Args&&... args);
        //     void report_results();
        // };
        
        cpp_mastery::Logger::info("TODO: Implement function performance analyzer");
    }
}