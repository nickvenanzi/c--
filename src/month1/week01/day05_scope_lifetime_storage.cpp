#include "cpp_mastery.hpp"
#include <memory>
#include <thread>
#include <chrono>

// =============================================================================
// DAY 5: Scope, Lifetime, and Storage Classes
// =============================================================================
// Topics Covered:
// - Local, global, and static variables
// - Storage duration and linkage
// - Name lookup and scope resolution
// - Memory leaks and dangling pointers
// =============================================================================

// Global variables for demonstration
int global_counter = 0;
const int global_constant = 42;
static int file_static_var = 100;  // Internal linkage - only visible in this file

// Forward declarations
void demonstrate_static_locals();
void demonstrate_scope_resolution();

TEST_CASE("Variable Scope and Lifetime", "[scope][lifetime][day5]") {
    
    SECTION("Local variable scope and lifetime") {
        std::cerr << "\n=== Local Variable Scope ===" << std::endl;
        
        int outer_var = 10;
        std::cerr << "Outer scope - outer_var: " << outer_var << std::endl;
        
        {
            int inner_var = 20;
            int outer_var = 30;  // Shadows outer scope variable
            
            std::cerr << "Inner scope - inner_var: " << inner_var << std::endl;
            std::cerr << "Inner scope - outer_var (shadowed): " << outer_var << std::endl;
            
            REQUIRE(inner_var == 20);
            REQUIRE(outer_var == 30);  // This is the shadowed version
        }
        // inner_var is destroyed here
        // outer_var shadows is destroyed here
        
        std::cerr << "Back to outer scope - outer_var: " << outer_var << std::endl;
        REQUIRE(outer_var == 10);  // Original value restored
        
        cpp_mastery::Logger::info("Local variable scope and shadowing demonstrated");
    }
    
    SECTION("Global variable access and modification") {
        std::cerr << "\n=== Global Variables ===" << std::endl;
        
        std::cerr << "Initial global_counter: " << global_counter << std::endl;
        std::cerr << "Global constant: " << global_constant << std::endl;
        std::cerr << "File static variable: " << file_static_var << std::endl;
        
        // Modify global variables
        global_counter += 10;
        file_static_var += 5;
        
        std::cerr << "After modification:" << std::endl;
        std::cerr << "  global_counter: " << global_counter << std::endl;
        std::cerr << "  file_static_var: " << file_static_var << std::endl;
        
        // Test global variable persistence
        REQUIRE(global_counter == 10);
        REQUIRE(file_static_var == 105);
        
        cpp_mastery::Logger::info("Global variable access demonstrated");
    }
    
    SECTION("Static local variables") {
        std::cerr << "\n=== Static Local Variables ===" << std::endl;
        
        // Function with static local variable
        auto increment_counter = []() -> int {
            static int counter = 0;  // Initialized only once
            counter++;
            std::cerr << "Static counter: " << counter << std::endl;
            return counter;
        };
        
        // Call multiple times to see static behavior
        int result1 = increment_counter();  // counter = 1
        int result2 = increment_counter();  // counter = 2
        int result3 = increment_counter();  // counter = 3
        
        REQUIRE(result1 == 1);
        REQUIRE(result2 == 2);
        REQUIRE(result3 == 3);
        
        // Demonstrate with external function
        demonstrate_static_locals();
        demonstrate_static_locals();
        demonstrate_static_locals();
        
        cpp_mastery::Logger::info("Static local variables demonstrated");
    }
    
    SECTION("Variable lifetime and destruction order") {
        std::cerr << "\n=== Variable Lifetime ===" << std::endl;
        
        // Class to demonstrate construction/destruction
        class LifetimeDemo {
        private:
            std::string name_;
            
        public:
            explicit LifetimeDemo(const std::string& name) : name_(name) {
                std::cerr << "Constructing " << name_ << std::endl;
            }
            
            ~LifetimeDemo() {
                std::cerr << "Destroying " << name_ << std::endl;
            }
            
            const std::string& name() const { return name_; }
        };
        
        std::cerr << "Creating objects in different scopes:" << std::endl;
        
        LifetimeDemo outer("outer");
        
        {
            LifetimeDemo inner1("inner1");
            LifetimeDemo inner2("inner2");
            
            std::cerr << "Inside inner scope with " << inner1.name() 
                     << " and " << inner2.name() << std::endl;
        }
        // inner2 destroyed first (reverse order of construction)
        // inner1 destroyed second
        
        std::cerr << "Back in outer scope with " << outer.name() << std::endl;
        
        cpp_mastery::Logger::info("Variable lifetime and destruction order demonstrated");
    }
}

// Function to demonstrate static locals
void demonstrate_static_locals() {
    static int call_count = 0;
    static std::string message = "Hello from static local";
    
    call_count++;
    std::cerr << "Function called " << call_count << " times. Message: " << message << std::endl;
}

TEST_CASE("Storage Duration Classes", "[storage][duration][day5]") {
    
    SECTION("Automatic storage duration") {
        std::cerr << "\n=== Automatic Storage Duration ===" << std::endl;
        
        // Automatic variables (stack allocated)
        int automatic_int = 42;
        double automatic_double = 3.14;
        std::string automatic_string = "Hello";
        
        std::cerr << "Automatic variables addresses:" << std::endl;
        std::cerr << "  int: " << &automatic_int << std::endl;
        std::cerr << "  double: " << &automatic_double << std::endl;
        std::cerr << "  string: " << &automatic_string << std::endl;
        
        // These variables are automatically destroyed when scope ends
        REQUIRE(automatic_int == 42);
        
        cpp_mastery::Logger::info("Automatic storage duration demonstrated");
    }
    
    SECTION("Static storage duration") {
        std::cerr << "\n=== Static Storage Duration ===" << std::endl;
        
        // Static variables (initialized once, persist for program lifetime)
        static int static_counter = 0;
        static std::string static_message = "Persistent message";
        
        static_counter++;
        
        std::cerr << "Static variables:" << std::endl;
        std::cerr << "  counter: " << static_counter << " at " << &static_counter << std::endl;
        std::cerr << "  message: " << static_message << " at " << &static_message << std::endl;
        
        // Static variables retain their values between function calls
        REQUIRE(static_counter >= 1);
        
        cpp_mastery::Logger::info("Static storage duration demonstrated");
    }
    
    SECTION("Dynamic storage duration") {
        std::cerr << "\n=== Dynamic Storage Duration ===" << std::endl;
        
        // Dynamic allocation (heap allocated)
        int* dynamic_int = new int(100);
        std::string* dynamic_string = new std::string("Dynamic string");
        
        std::cerr << "Dynamic variables addresses:" << std::endl;
        std::cerr << "  int: " << dynamic_int << " (value: " << *dynamic_int << ")" << std::endl;
        std::cerr << "  string: " << dynamic_string << " (value: " << *dynamic_string << ")" << std::endl;
        
        REQUIRE(*dynamic_int == 100);
        REQUIRE(*dynamic_string == "Dynamic string");
        
        // Manual cleanup required
        delete dynamic_int;
        delete dynamic_string;
        
        // Smart pointers for automatic cleanup
        std::unique_ptr<int> smart_int = std::make_unique<int>(200);
        std::unique_ptr<std::string> smart_string = std::make_unique<std::string>("Smart string");
        
        std::cerr << "Smart pointer values:" << std::endl;
        std::cerr << "  int: " << *smart_int << std::endl;
        std::cerr << "  string: " << *smart_string << std::endl;
        
        REQUIRE(*smart_int == 200);
        
        cpp_mastery::Logger::info("Dynamic storage duration demonstrated");
    }
    
    // TODO: Implement thread-local storage demonstration
    SECTION("Thread-local storage duration") {
        // TODO: Demonstrate thread_local variables
        // Requirements:
        // 1. Show thread_local variable behavior across threads
        // 2. Compare with static variables in multi-threaded context
        // 3. Demonstrate initialization and destruction per thread
        
        std::cerr << "\n=== Thread-Local Storage ===" << std::endl;
        
        // Variables to demonstrate the difference
        thread_local int tls_counter = 0;      // Each thread gets its own copy
        std::atomic<int> shared_counter{};         // Shared between all threads
        
        auto worker = [&](int thread_id) {
            for (int i = 0; i < 5; ++i) {
                tls_counter++;      // Each thread modifies its own copy
                shared_counter++;   // All threads modify the same variable
                
                std::cerr << "Thread " << thread_id 
                         << " - TLS counter: " << tls_counter 
                         << ", Shared counter: " << shared_counter << std::endl;
                
                // Small delay to see interleaving
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        // Create two threads
        std::thread t1(worker, 1);
        std::thread t2(worker, 2);
        
        // Wait for both threads to complete
        t1.join();
        t2.join();
        
        std::cerr << "Main thread - TLS counter: " << tls_counter 
                 << ", Final shared counter: " << shared_counter << std::endl;
        
        cpp_mastery::Logger::info("Thread-local storage demonstration completed");
    }
}

TEST_CASE("Linkage and Name Lookup", "[linkage][lookup][day5]") {
    
    SECTION("Internal vs external linkage") {
        std::cerr << "\n=== Linkage Types ===" << std::endl;
        
        // External linkage (can be accessed from other files)
        extern int external_var;  // Declaration (defined elsewhere)
        
        // Internal linkage (only visible in this file)
        static int internal_var = 42;
        
        // No linkage (local scope only)
        int local_var = 10;
        
        std::cerr << "Internal linkage variable: " << internal_var << std::endl;
        std::cerr << "Local variable: " << local_var << std::endl;
        std::cerr << "File static variable: " << file_static_var << std::endl;
        
        REQUIRE(internal_var == 42);
        REQUIRE(local_var == 10);
        
        cpp_mastery::Logger::info("Linkage types demonstrated");
    }
    
    SECTION("Name lookup and scope resolution") {
        std::cerr << "\n=== Name Lookup ===" << std::endl;
        
        int variable = 100;  // Local variable
        
        {
            int variable = 200;  // Shadows outer variable
            std::cerr << "Inner scope variable: " << variable << std::endl;
            
            // Access global variable using scope resolution
            std::cerr << "Global counter: " << ::global_counter << std::endl;
            
            REQUIRE(variable == 200);  // Local shadowed version
        }
        
        std::cerr << "Outer scope variable: " << variable << std::endl;
        REQUIRE(variable == 100);  // Original local variable
        
        demonstrate_scope_resolution();
        
        cpp_mastery::Logger::info("Name lookup and scope resolution demonstrated");
    }
    
    // TODO: Implement namespace scope examples
    SECTION("Namespace scope and using declarations") {
        // TODO: Demonstrate:
        // 1. Namespace definition and usage
        // 2. using declarations vs using directives
        // 3. Argument-dependent lookup (ADL)
        // 4. Anonymous namespaces
        
        // Example structure:
        // namespace MyNamespace {
        //     int value = 42;
        //     void function();
        // }
        // 
        // using MyNamespace::value;  // using declaration
        // using namespace MyNamespace;  // using directive
        
        cpp_mastery::Logger::info("TODO: Implement namespace scope examples");
    }
}

// Function to demonstrate scope resolution
void demonstrate_scope_resolution() {
    int global_counter = 999;  // Local variable shadows global
    
    std::cerr << "Local global_counter: " << global_counter << std::endl;
    std::cerr << "Global ::global_counter: " << ::global_counter << std::endl;
}

TEST_CASE("Memory Management and Dangling Pointers", "[memory][dangling][day5]") {
    
    SECTION("Stack vs heap memory lifetime") {
        std::cerr << "\n=== Stack vs Heap Lifetime ===" << std::endl;
        
        int* stack_ptr = nullptr;
        int* heap_ptr = nullptr;
        
        {
            int stack_var = 42;
            stack_ptr = &stack_var;  // Points to stack variable
            heap_ptr = new int(84);  // Points to heap variable
            
            std::cerr << "Inside scope:" << std::endl;
            std::cerr << "  Stack variable: " << *stack_ptr << std::endl;
            std::cerr << "  Heap variable: " << *heap_ptr << std::endl;
            
            REQUIRE(*stack_ptr == 42);
            REQUIRE(*heap_ptr == 84);
        }
        // stack_var is destroyed here, stack_ptr becomes dangling
        // heap_ptr still valid (but creates memory leak if not deleted)
        
        std::cerr << "Outside scope:" << std::endl;
        // std::cerr << "Stack variable: " << *stack_ptr << std::endl;  // DANGEROUS!
        std::cerr << "Heap variable: " << *heap_ptr << std::endl;  // Still valid
        
        REQUIRE(*heap_ptr == 84);  // Still accessible
        
        delete heap_ptr;  // Clean up heap memory
        heap_ptr = nullptr;
        
        cpp_mastery::Logger::info("Stack vs heap lifetime demonstrated");
    }
    
    SECTION("Dangling pointer detection and prevention") {
        std::cerr << "\n=== Dangling Pointer Prevention ===" << std::endl;
        
        // Demonstrate safe pointer management
        std::unique_ptr<int> safe_ptr = std::make_unique<int>(100);
        int* raw_ptr = safe_ptr.get();
        
        std::cerr << "Safe pointer value: " << *safe_ptr << std::endl;
        std::cerr << "Raw pointer value: " << *raw_ptr << std::endl;
        
        REQUIRE(*safe_ptr == 100);
        REQUIRE(*raw_ptr == 100);
        
        // When safe_ptr goes out of scope, memory is automatically freed
        // raw_ptr would become dangling, but we don't use it after this
        
        cpp_mastery::Logger::info("Dangling pointer prevention demonstrated");
    }
    
    SECTION("Memory leak detection") {
        std::cerr << "\n=== Memory Leak Prevention ===" << std::endl;
        
        // Bad: Manual memory management (potential leak)
        auto create_leak = []() -> int* {
            return new int(42);  // Caller must remember to delete
        };
        
        // Good: RAII with smart pointers
        auto create_safe = []() -> std::unique_ptr<int> {
            return std::make_unique<int>(42);  // Automatic cleanup
        };
        
        // Demonstrate leak-prone code
        int* potential_leak = create_leak();
        std::cerr << "Potential leak value: " << *potential_leak << std::endl;
        delete potential_leak;  // Must remember this!
        
        // Demonstrate safe code
        auto safe_resource = create_safe();
        std::cerr << "Safe resource value: " << *safe_resource << std::endl;
        // Automatic cleanup when safe_resource goes out of scope
        
        cpp_mastery::Logger::info("Memory leak prevention demonstrated");
    }
    
    // TODO: Implement advanced memory management patterns
    SECTION("Advanced memory management patterns") {
        // TODO: Implement:
        // 1. RAII (Resource Acquisition Is Initialization) patterns
        // 2. Custom smart pointer implementation
        // 3. Memory pool allocation
        // 4. Weak references to break circular dependencies
        
        // Example RAII class:
        // class FileHandle {
        //     FILE* file_;
        // public:
        //     FileHandle(const char* filename) : file_(fopen(filename, "r")) {}
        //     ~FileHandle() { if (file_) fclose(file_); }
        //     FILE* get() const { return file_; }
        // };
        
        cpp_mastery::Logger::info("TODO: Implement advanced memory management patterns");
    }
}

TEST_CASE("Static Initialization and Destruction", "[static][initialization][day5]") {
    
    SECTION("Static initialization order") {
        std::cerr << "\n=== Static Initialization Order ===" << std::endl;
        
        // Class to demonstrate static initialization
        class StaticDemo {
        private:
            std::string name_;
            
        public:
            explicit StaticDemo(const std::string& name) : name_(name) {
                std::cerr << "Static object " << name_ << " constructed" << std::endl;
            }
            
            ~StaticDemo() {
                std::cerr << "Static object " << name_ << " destroyed" << std::endl;
            }
            
            const std::string& name() const { return name_; }
        };
        
        // Static objects in function scope (initialized on first call)
        static StaticDemo func_static1("func_static1");
        static StaticDemo func_static2("func_static2");
        
        std::cerr << "Function static objects: " << func_static1.name() 
                 << ", " << func_static2.name() << std::endl;
        
        cpp_mastery::Logger::info("Static initialization order demonstrated");
    }
    
    SECTION("Static destruction order") {
        std::cerr << "\n=== Static Destruction Order ===" << std::endl;
        
        // Function that creates static objects
        auto create_static = [](const std::string& name) -> void {
            class LocalStatic {
            private:
                std::string name_;
            public:
                explicit LocalStatic(const std::string& n) : name_(n) {
                    std::cerr << "Creating static " << name_ << std::endl;
                }
                ~LocalStatic() {
                    std::cerr << "Destroying static " << name_ << std::endl;
                }
            };
            
            static LocalStatic obj(name);
        };
        
        // Create static objects in different functions
        create_static("first");
        create_static("second");
        create_static("third");
        
        // Static objects are destroyed in reverse order of construction
        // when the program exits
        
        cpp_mastery::Logger::info("Static destruction order will be shown at program exit");
    }
    
    // TODO: Implement static initialization fiasco examples
    SECTION("Static initialization fiasco") {
        // TODO: Demonstrate:
        // 1. Problems with static initialization order across translation units
        // 2. Solutions using local static variables
        // 3. Singleton pattern implementation
        // 4. Meyer's singleton vs other approaches
        
        // Example problem:
        // // file1.cpp
        // static MyClass global1;
        // 
        // // file2.cpp  
        // static MyClass global2(&global1);  // Depends on global1 being initialized first!
        
        cpp_mastery::Logger::info("TODO: Implement static initialization fiasco examples");
    }
}

TEST_CASE("Performance Implications of Storage Classes", "[performance][storage][day5]") {
    
    SECTION("Access time comparison") {
        const size_t iterations = 10000000;
        
        // Local variable (stack)
        int local_var = 0;
        
        // Static variable
        static int static_var = 0;
        
        // Global variable access
        global_counter = 0;
        
        BENCHMARK("Local variable access") {
            for (size_t i = 0; i < iterations; ++i) {
                local_var += static_cast<int>(i);
            }
            return local_var;
        };
        
        BENCHMARK("Static variable access") {
            for (size_t i = 0; i < iterations; ++i) {
                static_var += static_cast<int>(i);
            }
            return static_var;
        };
        
        BENCHMARK("Global variable access") {
            for (size_t i = 0; i < iterations; ++i) {
                global_counter += static_cast<int>(i);
            }
            return global_counter;
        };
        
        cpp_mastery::Logger::info("Variable access performance compared");
    }
    
    SECTION("Memory allocation performance") {
        const size_t iterations = 100000;
        
        BENCHMARK("Stack allocation") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                int stack_array[100];  // Stack allocation
                stack_array[0] = static_cast<int>(i);
                sum += stack_array[0];
            }
            return sum;
        };
        
        BENCHMARK("Heap allocation") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                int* heap_array = new int[100];  // Heap allocation
                heap_array[0] = static_cast<int>(i);
                sum += heap_array[0];
                delete[] heap_array;
            }
            return sum;
        };
        
        BENCHMARK("Smart pointer allocation") {
            volatile int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                auto smart_array = std::make_unique<int[]>(100);  // Smart pointer
                smart_array[0] = static_cast<int>(i);
                sum += smart_array[0];
                // Automatic cleanup
            }
            return sum;
        };
        
        cpp_mastery::Logger::info("Memory allocation performance compared");
    }
    
    // TODO: Implement cache locality analysis
    SECTION("Cache locality and storage classes") {
        // TODO: Analyze cache performance of different storage classes:
        // 1. Local variables (stack) - good cache locality
        // 2. Static variables - may have poor cache locality
        // 3. Global variables - depends on access patterns
        // 4. Heap allocated - depends on allocation patterns
        
        cpp_mastery::Logger::info("TODO: Implement cache locality analysis");
    }
}
