#include "cpp_mastery.hpp"

TEST_CASE("Cast Performance Analysis", "[cast][benchmark]") {
    const size_t array_size = 1000;
    int stack_array[1000];

    BENCHMARK("No cast, int literals") {
        for (int i = 0; i < 1000; ++i) {  // Direct int literal
            stack_array[i] = i;
        }
        cpp_mastery::do_not_optimize(stack_array);
        return stack_array[999];
    };
    
    BENCHMARK("Cast in loop") {
        for (size_t i = 0; i < array_size; ++i) {
            stack_array[i] = static_cast<int>(i);  // Cast every iteration
        }
        cpp_mastery::do_not_optimize(stack_array);
        return stack_array[999];
    };
    
    BENCHMARK("Cast once, int loop") {
        const int int_array_size = static_cast<int>(array_size);
        for (int i = 0; i < int_array_size; ++i) {
            stack_array[i] = i;  // No cast needed
        }
        cpp_mastery::do_not_optimize(stack_array);
        return stack_array[999];
    };
}