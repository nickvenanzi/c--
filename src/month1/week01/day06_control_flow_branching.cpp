#include "cpp_mastery.hpp"
#include <random>
#include <algorithm>
#include <chrono>

// =============================================================================
// DAY 6: Control Flow and Branching
// =============================================================================
// Topics Covered:
// - if/else optimization and branch prediction
// - Switch statements and jump tables
// - Loop types and performance characteristics
// - Break, continue, and goto
// =============================================================================

TEST_CASE("Conditional Statements and Branch Prediction", "[control][branching][day6]") {
    
    SECTION("Basic if/else statements") {
        std::cerr << "\n=== Basic Conditional Statements ===" << std::endl;
        
        auto classify_number = [](int x) -> std::string {
            if (x > 0) {
                return "positive";
            } else if (x < 0) {
                return "negative";
            } else {
                return "zero";
            }
        };
        
        // Test different cases
        std::vector<int> test_values = {-5, 0, 10, -1, 42};
        for (int val : test_values) {
            std::string result = classify_number(val);
            std::cerr << "classify_number(" << val << ") = " << result << std::endl;
        }
        
        REQUIRE(classify_number(5) == "positive");
        REQUIRE(classify_number(-3) == "negative");
        REQUIRE(classify_number(0) == "zero");
        
        cpp_mastery::Logger::info("Basic conditional statements demonstrated");
    }
    
    // SECTION("Branch prediction performance") {
    //     const size_t size = 1000000;
    //     const size_t iterations = 100;
        
    //     // Create predictable data (sorted)
    //     std::vector<int> predictable_data(size);
    //     std::iota(predictable_data.begin(), predictable_data.end(), 0);
        
    //     // Create unpredictable data (random)
    //     std::vector<int> unpredictable_data = predictable_data;
    //     std::random_device rd;
    //     std::mt19937 gen(42);  // Fixed seed for reproducibility
    //     std::shuffle(unpredictable_data.begin(), unpredictable_data.end(), gen);
        
    //     const int threshold = static_cast<int>(size / 2);
        
    //     BENCHMARK("Predictable branches (sorted data)") {
    //         volatile long sum = 0;
    //         for (size_t iter = 0; iter < iterations; ++iter) {
    //             for (int value : predictable_data) {
    //                 if (value < threshold) {
    //                     sum += value;  // Predictable branch
    //                 } else {
    //                     sum += value * 2;  // Predictable branch
    //                 }
    //             }
    //         }
    //         return sum;
    //     };
        
    //     BENCHMARK("Unpredictable branches (random data)") {
    //         volatile long sum = 0;
    //         for (size_t iter = 0; iter < iterations; ++iter) {
    //             for (int value : unpredictable_data) {
    //                 if (value < threshold) {
    //                     sum += value;  // Unpredictable branch
    //                 } else {
    //                     sum += value * 2;  // Unpredictable branch
    //                 }
    //             }
    //         }
    //         return sum;
    //     };
        
    //     cpp_mastery::Logger::info("Branch prediction performance compared");
    // }
    
    // SECTION("Branchless programming techniques") {
    //     const size_t size = 1000000;
    //     const size_t iterations = 100;
        
    //     std::vector<int> data = cpp_mastery::generate_random_data<int>(size, -1000, 1000);
        
    //     BENCHMARK("Branched absolute value") {
    //         volatile long sum = 0;
    //         for (size_t iter = 0; iter < iterations; ++iter) {
    //             for (int value : data) {
    //                 int abs_val = (value < 0) ? -value : value;  // Branch
    //                 sum += abs_val;
    //             }
    //         }
    //         return sum;
    //     };
        
    //     BENCHMARK("Branchless absolute value") {
    //         volatile long sum = 0;
    //         for (size_t iter = 0; iter < iterations; ++iter) {
    //             for (int value : data) {
    //                 int mask = value >> 31;  // Arithmetic right shift
    //                 int abs_val = (value + mask) ^ mask;  // Branchless
    //                 sum += abs_val;
    //             }
    //         }
    //         return sum;
    //     };
        
    //     BENCHMARK("Standard library abs") {
    //         volatile long sum = 0;
    //         for (size_t iter = 0; iter < iterations; ++iter) {
    //             for (int value : data) {
    //                 int abs_val = std::abs(value);  // Optimized implementation
    //                 sum += abs_val;
    //             }
    //         }
    //         return sum;
    //     };
        
    //     cpp_mastery::Logger::info("Branchless programming techniques compared");
    // }
}

TEST_CASE("Switch Statements and Jump Tables", "[switch][jumptable][day6]") {
    
    SECTION("Basic switch statement") {
        std::cerr << "\n=== Switch Statements ===" << std::endl;
        
        auto get_day_name = [](int day) -> std::string {
            switch (day) {
                case 1: return "Monday";
                case 2: return "Tuesday";
                case 3: return "Wednesday";
                case 4: return "Thursday";
                case 5: return "Friday";
                case 6: return "Saturday";
                case 7: return "Sunday";
                default: return "Invalid day";
            }
        };
        
        // Test switch statement
        for (int i = 1; i <= 8; ++i) {
            std::string day_name = get_day_name(i);
            std::cerr << "Day " << i << ": " << day_name << std::endl;
        }
        
        REQUIRE(get_day_name(1) == "Monday");
        REQUIRE(get_day_name(7) == "Sunday");
        REQUIRE(get_day_name(8) == "Invalid day");
        
        cpp_mastery::Logger::info("Basic switch statement demonstrated");
    }
    
    SECTION("Switch vs if-else performance") {
        const size_t iterations = 1000;
        
        // Generate random values for testing
        std::vector<int> test_values(1000);
        std::random_device rd;
        std::mt19937 gen(42);
        std::uniform_int_distribution<> dis(1, 10);
        std::generate(test_values.begin(), test_values.end(), [&]() { return dis(gen); });
        
        // Switch-based implementation
        auto switch_classify = [](int value) -> int {
            switch (value) {
                case 1: case 2: case 3: return 1;  // Small
                case 4: case 5: case 6: return 2;  // Medium
                case 7: case 8: case 9: return 3;  // Large
                case 10: return 4;                 // Extra large
                default: return 0;                 // Invalid
            }
        };
        
        // If-else chain implementation
        auto ifelse_classify = [](int value) -> int {
            if (value >= 1 && value <= 3) return 1;
            else if (value >= 4 && value <= 6) return 2;
            else if (value >= 7 && value <= 9) return 3;
            else if (value == 10) return 4;
            else return 0;
        };
        
        BENCHMARK("Switch statement") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (int value : test_values) {
                    sum += switch_classify(value);
                }
            }
            return sum;
        };
        
        BENCHMARK("If-else chain") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (int value : test_values) {
                    sum += ifelse_classify(value);
                }
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Switch vs if-else performance compared");
    }
    
    SECTION("Jump table optimization") {
        std::cerr << "\n=== Jump Table Optimization ===" << std::endl;
        
        // Dense switch (good for jump table)
        auto dense_switch = [](int x) -> int {
            switch (x) {
                case 0: return x * 1;
                case 1: return x * 2;
                case 2: return x * 3;
                case 3: return x * 4;
                case 4: return x * 5;
                case 5: return x * 6;
                case 6: return x * 7;
                case 7: return x * 8;
                default: return 0;
            }
        };
        
        // Sparse switch (less suitable for jump table)
        auto sparse_switch = [](int x) -> int {
            switch (x) {
                case 1: return x * 1;
                case 100: return x * 2;
                case 1000: return x * 3;
                case 10000: return x * 4;
                default: return 0;
            }
        };
        
        // Test dense switch
        std::cerr << "Dense switch results:" << std::endl;
        for (int i = 0; i < 8; ++i) {
            int result = dense_switch(i);
            std::cerr << "  dense_switch(" << i << ") = " << result << std::endl;
        }
        
        // Test sparse switch
        std::cerr << "Sparse switch results:" << std::endl;
        std::vector<int> sparse_values = {1, 100, 1000, 10000, 50};
        for (int val : sparse_values) {
            int result = sparse_switch(val);
            std::cerr << "  sparse_switch(" << val << ") = " << result << std::endl;
        }
        
        cpp_mastery::Logger::info("Jump table optimization demonstrated");
    }
}

TEST_CASE("Loop Types and Performance", "[loops][performance][day6]") {
    
    SECTION("Basic loop types") {
        std::cerr << "\n=== Basic Loop Types ===" << std::endl;
        
        const size_t size = 10;
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        
        // For loop
        std::cerr << "For loop: ";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cerr << data[i] << " ";
        }
        std::cerr << std::endl;
        
        // Range-based for loop (C++11)
        std::cerr << "Range-based for: ";
        for (int value : data) {
            std::cerr << value << " ";
        }
        std::cerr << std::endl;
        
        // While loop
        std::cerr << "While loop: ";
        size_t i = 0;
        while (i < data.size()) {
            std::cerr << data[i] << " ";
            ++i;
        }
        std::cerr << std::endl;
        
        // Do-while loop
        std::cerr << "Do-while loop: ";
        i = 0;
        do {
            std::cerr << data[i] << " ";
            ++i;
        } while (i < data.size());
        std::cerr << std::endl;
        
        cpp_mastery::Logger::info("Basic loop types demonstrated");
    }
    
    SECTION("Loop performance comparison") {
        const size_t size = 1000;
        const size_t iterations = 100;
        
        std::vector<int> data = cpp_mastery::generate_sequential_data<int>(size, 1);
        
        BENCHMARK("Traditional for loop") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (size_t i = 0; i < data.size(); ++i) {
                    sum += data[i];
                }
            }
            return sum;
        };
        
        BENCHMARK("Range-based for loop") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (int value : data) {
                    sum += value;
                }
            }
            return sum;
        };
        
        BENCHMARK("Iterator-based loop") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (auto it = data.begin(); it != data.end(); ++it) {
                    sum += *it;
                }
            }
            return sum;
        };
        
        BENCHMARK("While loop") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                size_t i = 0;
                while (i < data.size()) {
                    sum += data[i];
                    ++i;
                }
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Loop performance compared");
    }
    
    SECTION("Loop unrolling demonstration") {
        const size_t size = 1000000;
        const size_t iterations = 1;
        
        std::vector<int> data = cpp_mastery::generate_sequential_data<int>(size, 1);
        
        BENCHMARK("Normal loop") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (size_t i = 0; i < data.size(); ++i) {
                    sum += data[i];
                }
            }
            return sum;
        };
        
        BENCHMARK("Manually unrolled loop (4x)") {
            volatile long sum = 0;
            for (size_t iter = 0; iter < iterations; ++iter) {
                size_t i = 0;
                for (; i + 3 < data.size(); i += 4) {
                    sum += data[i];
                    sum += data[i + 1];
                    sum += data[i + 2];
                    sum += data[i + 3];
                }
                // Handle remaining elements
                for (; i < data.size(); ++i) {
                    sum += data[i];
                }
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Loop unrolling performance compared");
    }
}

TEST_CASE("Break, Continue, and Goto", "[break][continue][goto][day6]") {
    
    SECTION("Break and continue in loops") {
        std::cerr << "\n=== Break and Continue ===" << std::endl;
        
        // Demonstrate break
        std::cerr << "Break example (find first even number > 10): ";
        for (int i = 1; i <= 20; ++i) {
            if (i % 2 == 0 && i > 10) {
                std::cerr << i << std::endl;
                break;  // Exit loop immediately
            }
        }
        
        // Demonstrate continue
        std::cerr << "Continue example (odd numbers only): ";
        for (int i = 1; i <= 10; ++i) {
            if (i % 2 == 0) {
                continue;  // Skip to next iteration
            }
            std::cerr << i << " ";
        }
        std::cerr << std::endl;
        
        // Nested loop break/continue
        std::cerr << "Nested loop with break/continue:" << std::endl;
        for (int i = 1; i <= 3; ++i) {
            std::cerr << "Outer loop i=" << i << ": ";
            for (int j = 1; j <= 5; ++j) {
                if (j == 3) continue;  // Skip j=3
                if (j == 5) break;     // Exit inner loop at j=5
                std::cerr << j << " ";
            }
            std::cerr << std::endl;
        }
        
        cpp_mastery::Logger::info("Break and continue demonstrated");
    }
    
    SECTION("Labeled break and continue (goto alternative)") {
        std::cerr << "\n=== Labeled Break Alternative ===" << std::endl;
        
        // Using goto to break out of nested loops
        bool found = false;
        int target = 15;
        int found_i = -1, found_j = -1;
        
        for (int i = 1; i <= 5 && !found; ++i) {
            for (int j = 1; j <= 5; ++j) {
                if (i * j == target) {
                    found_i = i;
                    found_j = j;
                    found = true;
                    break;  // Only breaks inner loop
                }
            }
        }
        
        if (found) {
            std::cerr << "Found " << target << " = " << found_i << " * " << found_j << std::endl;
        }
        
        // Alternative using goto (generally discouraged)
        found = false;
        for (int i = 1; i <= 5; ++i) {
            for (int j = 1; j <= 5; ++j) {
                if (i * j == target) {
                    found_i = i;
                    found_j = j;
                    goto found_target;  // Jump out of both loops
                }
            }
        }
        
        found_target:
        std::cerr << "Using goto - Found " << target << " = " << found_i << " * " << found_j << std::endl;
        
        cpp_mastery::Logger::info("Labeled break alternatives demonstrated");
    }
    
    SECTION("Goto for error handling (legacy pattern)") {
        std::cerr << "\n=== Goto for Error Handling ===" << std::endl;
        
        // Simulate C-style error handling with goto
        auto legacy_function = []() -> int {
            int* buffer1 = nullptr;
            int* buffer2 = nullptr;
            int result = -1;
            
            // Allocate resources
            buffer1 = new(std::nothrow) int[100];
            if (!buffer1) {
                std::cerr << "Failed to allocate buffer1" << std::endl;
                goto cleanup;
            }
            
            buffer2 = new(std::nothrow) int[200];
            if (!buffer2) {
                std::cerr << "Failed to allocate buffer2" << std::endl;
                goto cleanup;
            }
            
            // Do work
            buffer1[0] = 42;
            buffer2[0] = 84;
            result = buffer1[0] + buffer2[0];
            std::cerr << "Computation successful: " << result << std::endl;
            
        cleanup:
            // Clean up resources (executed regardless of where we came from)
            delete[] buffer1;
            delete[] buffer2;
            return result;
        };
        
        // Modern RAII alternative
        auto modern_function = []() -> int {
            try {
                std::unique_ptr<int[]> buffer1 = std::make_unique<int[]>(100);
                std::unique_ptr<int[]> buffer2 = std::make_unique<int[]>(200);
                
                buffer1[0] = 42;
                buffer2[0] = 84;
                int result = buffer1[0] + buffer2[0];
                std::cerr << "Modern computation successful: " << result << std::endl;
                return result;
            } catch (const std::bad_alloc& e) {
                std::cerr << "Allocation failed: " << e.what() << std::endl;
                return -1;
            }
        };
        
        int legacy_result = legacy_function();
        int modern_result = modern_function();
        
        REQUIRE(legacy_result == modern_result);
        
        cpp_mastery::Logger::info("Goto vs RAII error handling compared");
    }
}

TEST_CASE("Advanced Control Flow Patterns", "[advanced][control][day6]") {
    
    SECTION("State machine implementation") {
        std::cerr << "\n=== State Machine Implementation ===" << std::endl;
        
        enum class State { IDLE, RUNNING, PAUSED, STOPPED };
        enum class Event { START, PAUSE, RESUME, STOP };
        
        class StateMachine {
        private:
            State current_state_ = State::IDLE;
            
        public:
            void process_event(Event event) {
                switch (current_state_) {
                    case State::IDLE:
                        switch (event) {
                            case Event::START:
                                current_state_ = State::RUNNING;
                                std::cerr << "IDLE -> RUNNING" << std::endl;
                                break;
                            default:
                                std::cerr << "Invalid event in IDLE state" << std::endl;
                                break;
                        }
                        break;
                        
                    case State::RUNNING:
                        switch (event) {
                            case Event::PAUSE:
                                current_state_ = State::PAUSED;
                                std::cerr << "RUNNING -> PAUSED" << std::endl;
                                break;
                            case Event::STOP:
                                current_state_ = State::STOPPED;
                                std::cerr << "RUNNING -> STOPPED" << std::endl;
                                break;
                            default:
                                std::cerr << "Invalid event in RUNNING state" << std::endl;
                                break;
                        }
                        break;
                        
                    case State::PAUSED:
                        switch (event) {
                            case Event::RESUME:
                                current_state_ = State::RUNNING;
                                std::cerr << "PAUSED -> RUNNING" << std::endl;
                                break;
                            case Event::STOP:
                                current_state_ = State::STOPPED;
                                std::cerr << "PAUSED -> STOPPED" << std::endl;
                                break;
                            default:
                                std::cerr << "Invalid event in PAUSED state" << std::endl;
                                break;
                        }
                        break;
                        
                    case State::STOPPED:
                        std::cerr << "Machine is stopped" << std::endl;
                        break;
                }
            }
            
            State get_state() const { return current_state_; }
        };
        
        // Test state machine
        StateMachine sm;
        sm.process_event(Event::START);   // IDLE -> RUNNING
        sm.process_event(Event::PAUSE);   // RUNNING -> PAUSED
        sm.process_event(Event::RESUME);  // PAUSED -> RUNNING
        sm.process_event(Event::STOP);    // RUNNING -> STOPPED
        
        REQUIRE(sm.get_state() == State::STOPPED);
        
        cpp_mastery::Logger::info("State machine implementation demonstrated");
    }
    
    SECTION("Exception-based control flow") {
        std::cerr << "\n=== Exception-Based Control Flow ===" << std::endl;
        
        // Custom exception for control flow
        class EarlyReturn : public std::exception {
        private:
            int value_;
        public:
            explicit EarlyReturn(int val) : value_(val) {}
            int value() const { return value_; }
        };
        
        auto complex_computation = [](const std::vector<int>& data) -> int {
            try {
                int sum = 0;
                for (size_t i = 0; i < data.size(); ++i) {
                    sum += data[i];
                    
                    // Early return condition
                    if (sum > 1000) {
                        throw EarlyReturn(sum);  // Non-local exit
                    }
                    
                    // More complex processing...
                    sum *= 2;
                    
                    if (sum < 0) {  // Overflow check
                        throw std::overflow_error("Integer overflow");
                    }
                }
                return sum;
            } catch (const EarlyReturn& early) {
                std::cerr << "Early return with value: " << early.value() << std::endl;
                return early.value();
            }
        };
        
        std::vector<int> test_data = {10, 20, 30, 40, 50};
        int result = complex_computation(test_data);
        std::cerr << "Final result: " << result << std::endl;
        
        cpp_mastery::Logger::info("Exception-based control flow demonstrated");
    }
}