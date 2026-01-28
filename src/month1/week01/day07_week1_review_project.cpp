#include "cpp_mastery.hpp"
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstring>

// =============================================================================
// DAY 7: Week 1 Review - Two Implementation Projects
// =============================================================================
// PROJECT 1: Custom String Class with Copy-on-Write (COW)
// PROJECT 2: Simple Memory Manager with Pool Allocation
//
// You will implement both classes according to the specifications below.
// Comprehensive test suites are provided to verify your implementations.
// Performance benchmarks compare your implementations against standard library.
// =============================================================================

// =============================================================================
// PROJECT 1 SPECIFICATION: COWString Class
// =============================================================================

class COWString {
public:
    // Constructors and destructor
    COWString();
    explicit COWString(const char* str);
    COWString(const COWString& other);
    COWString& operator=(const COWString& other);
    ~COWString();
    
    // Access methods
    const char* c_str() const;
    size_t length() const;
    size_t size() const;
    bool empty() const;
    const char& operator[](size_t index) const;
    char& operator[](size_t index);
    
    // String operations
    COWString operator+(const COWString& other) const;
    bool operator==(const COWString& other) const;
    bool operator!=(const COWString& other) const;
    
    // Debug/testing methods
    size_t ref_count() const;
    const void* data_ptr() const;

private:
    // TODO: Implement your data structure here
    // Suggested: StringData* data_;
    void detach();
};

// =============================================================================
// PROJECT 2 SPECIFICATION: SimpleMemoryManager Class
// =============================================================================

class SimpleMemoryManager {
public:
    explicit SimpleMemoryManager(size_t pool_size);
    ~SimpleMemoryManager();
    
    void* allocate(size_t size);
    void deallocate(void* ptr);
    
    size_t get_allocated_bytes() const;
    size_t get_allocation_count() const;
    size_t get_deallocation_count() const;
    size_t get_free_block_count() const;
    size_t get_largest_free_block() const;

private:
    // TODO: Implement your data structure here
};

// =============================================================================
// TEST SUITE FOR COWString
// =============================================================================

TEST_CASE("COWString Implementation Tests", "[cowstring][project1][day7]") {
    
    SECTION("Basic construction and destruction") {
        std::cerr << "\n=== COWString Basic Construction ===" << std::endl;
        
        // Default constructor
        COWString empty;
        REQUIRE(empty.length() == 0);
        REQUIRE(empty.empty() == true);
        REQUIRE(std::strlen(empty.c_str()) == 0);
        
        // Constructor from C-string
        COWString hello("Hello");
        REQUIRE(hello.length() == 5);
        REQUIRE(hello.empty() == false);
        REQUIRE(std::strcmp(hello.c_str(), "Hello") == 0);
        
        // Constructor from nullptr
        COWString null_str(nullptr);
        REQUIRE(null_str.length() == 0);
        REQUIRE(null_str.empty() == true);
        
        std::cerr << "Basic construction tests passed" << std::endl;
    }
    
    SECTION("Copy-on-Write behavior") {
        std::cerr << "\n=== COWString Copy-on-Write ===" << std::endl;
        
        COWString original("Hello World");
        std::cerr << "Original: \"" << original.c_str() << "\" (refs: " << original.ref_count() << ")" << std::endl;
        
        // Copy constructor should share data
        COWString copy1 = original;
        std::cerr << "After copy construction:" << std::endl;
        std::cerr << "  Original refs: " << original.ref_count() << std::endl;
        std::cerr << "  Copy1 refs: " << copy1.ref_count() << std::endl;
        std::cerr << "  Data pointers same: " << (original.data_ptr() == copy1.data_ptr()) << std::endl;
        
        REQUIRE(original.ref_count() == 2);
        REQUIRE(copy1.ref_count() == 2);
        REQUIRE(original.data_ptr() == copy1.data_ptr());
        
        // Assignment should also share data
        COWString copy2;
        copy2 = original;
        REQUIRE(original.ref_count() == 3);
        REQUIRE(copy2.ref_count() == 3);
        REQUIRE(original.data_ptr() == copy2.data_ptr());
        
        // Non-const access should trigger COW
        copy1[0] = 'h';  // Should detach copy1
        std::cerr << "After modification:" << std::endl;
        std::cerr << "  Original: \"" << original.c_str() << "\" (refs: " << original.ref_count() << ")" << std::endl;
        std::cerr << "  Copy1: \"" << copy1.c_str() << "\" (refs: " << copy1.ref_count() << ")" << std::endl;
        std::cerr << "  Copy2: \"" << copy2.c_str() << "\" (refs: " << copy2.ref_count() << ")" << std::endl;
        
        REQUIRE(original.ref_count() == 2);  // original and copy2 still share
        REQUIRE(copy1.ref_count() == 1);     // copy1 is now separate
        REQUIRE(copy2.ref_count() == 2);     // copy2 still shares with original
        REQUIRE(original.data_ptr() != copy1.data_ptr());  // Different data
        REQUIRE(original.data_ptr() == copy2.data_ptr());  // Still sharing
        
        std::cerr << "Copy-on-Write tests passed" << std::endl;
    }
    
    SECTION("String operations") {
        std::cerr << "\n=== COWString Operations ===" << std::endl;
        
        COWString str1("Hello");
        COWString str2(" World");
        COWString str3 = str1 + str2;
        
        REQUIRE(str3.length() == 11);
        REQUIRE(std::strcmp(str3.c_str(), "Hello World") == 0);
        
        // Test equality
        COWString str4("Hello World");
        REQUIRE(str3 == str4);
        REQUIRE(!(str3 != str4));
        
        COWString str5("Different");
        REQUIRE(str3 != str5);
        REQUIRE(!(str3 == str5));
        
        // Test indexing
        REQUIRE(str3[0] == 'H');
        REQUIRE(str3[6] == 'W');
        
        std::cerr << "String operations tests passed" << std::endl;
    }
    
    SECTION("Edge cases and error handling") {
        std::cerr << "\n=== COWString Edge Cases ===" << std::endl;
        
        // Empty string operations
        COWString empty1;
        COWString empty2("");
        COWString empty3(nullptr);
        
        REQUIRE(empty1 == empty2);
        REQUIRE(empty2 == empty3);
        
        // Concatenation with empty strings
        COWString hello("Hello");
        COWString result1 = hello + empty1;
        COWString result2 = empty1 + hello;
        
        REQUIRE(result1 == hello);
        REQUIRE(result2 == hello);
        
        // Self-assignment
        COWString self_test("Test");
        size_t original_refs = self_test.ref_count();
        self_test = self_test;
        REQUIRE(self_test.ref_count() == original_refs);
        REQUIRE(std::strcmp(self_test.c_str(), "Test") == 0);
        
        std::cerr << "Edge cases tests passed" << std::endl;
    }
}

// =============================================================================
// TEST SUITE FOR SimpleMemoryManager
// =============================================================================

TEST_CASE("SimpleMemoryManager Implementation Tests", "[memorymanager][project2][day7]") {
    
    SECTION("Basic allocation and deallocation") {
        std::cerr << "\n=== Memory Manager Basic Operations ===" << std::endl;
        
        SimpleMemoryManager manager(1024);  // 1KB pool
        
        // Test basic allocation
        void* ptr1 = manager.allocate(100);
        REQUIRE(ptr1 != nullptr);
        REQUIRE(manager.get_allocated_bytes() >= 100);
        REQUIRE(manager.get_allocation_count() == 1);
        
        void* ptr2 = manager.allocate(200);
        REQUIRE(ptr2 != nullptr);
        REQUIRE(ptr2 != ptr1);  // Different pointers
        REQUIRE(manager.get_allocation_count() == 2);
        
        // Test memory usage
        std::memset(ptr1, 0xAA, 100);
        std::memset(ptr2, 0xBB, 200);
        
        // Verify memory is usable
        REQUIRE(static_cast<unsigned char*>(ptr1)[0] == 0xAA);
        REQUIRE(static_cast<unsigned char*>(ptr2)[0] == 0xBB);
        
        // Test deallocation
        manager.deallocate(ptr1);
        REQUIRE(manager.get_deallocation_count() == 1);
        
        manager.deallocate(ptr2);
        REQUIRE(manager.get_deallocation_count() == 2);
        
        std::cerr << "Basic operations tests passed" << std::endl;
    }
    
    SECTION("Block reuse and fragmentation") {
        std::cerr << "\n=== Memory Manager Block Reuse ===" << std::endl;
        
        SimpleMemoryManager manager(1024);
        
        // Allocate several blocks
        void* ptr1 = manager.allocate(100);
        void* ptr2 = manager.allocate(100);
        void* ptr3 = manager.allocate(100);
        
        REQUIRE(ptr1 != nullptr);
        REQUIRE(ptr2 != nullptr);
        REQUIRE(ptr3 != nullptr);
        
        // Free middle block
        manager.deallocate(ptr2);
        
        // Allocate block that should fit in freed space
        void* ptr4 = manager.allocate(50);  // Smaller than freed block
        REQUIRE(ptr4 != nullptr);
        
        // Check fragmentation
        size_t free_blocks = manager.get_free_block_count();
        std::cerr << "Free blocks after reallocation: " << free_blocks << std::endl;
        
        // Clean up
        manager.deallocate(ptr1);
        manager.deallocate(ptr3);
        manager.deallocate(ptr4);
        
        std::cerr << "Block reuse tests passed" << std::endl;
    }
    
    SECTION("Alignment and edge cases") {
        std::cerr << "\n=== Memory Manager Alignment ===" << std::endl;
        
        SimpleMemoryManager manager(1024);
        
        // Test alignment
        void* ptr1 = manager.allocate(1);
        void* ptr2 = manager.allocate(1);
        
        REQUIRE(ptr1 != nullptr);
        REQUIRE(ptr2 != nullptr);
        
        // Check pointer alignment
        uintptr_t addr1 = reinterpret_cast<uintptr_t>(ptr1);
        uintptr_t addr2 = reinterpret_cast<uintptr_t>(ptr2);
        
        REQUIRE(addr1 % sizeof(void*) == 0);  // Should be aligned
        REQUIRE(addr2 % sizeof(void*) == 0);  // Should be aligned
        
        // Test nullptr handling
        manager.deallocate(nullptr);  // Should not crash
        REQUIRE(manager.get_deallocation_count() == 0);  // Should not count nullptr
        
        // Test allocation failure
        void* huge_ptr = manager.allocate(2048);  // Larger than pool
        REQUIRE(huge_ptr == nullptr);
        
        // Clean up
        manager.deallocate(ptr1);
        manager.deallocate(ptr2);
        
        std::cerr << "Alignment and edge cases tests passed" << std::endl;
    }
}

// =============================================================================
// PERFORMANCE BENCHMARKS
// =============================================================================

TEST_CASE("Performance Benchmarks", "[benchmark][performance][day7]") {
    
    SECTION("COWString vs std::string performance") {
        std::cerr << "\n=== COWString Performance Benchmark ===" << std::endl;
        
        const size_t iterations = 100000;
        const char* test_string = "This is a test string for performance comparison";
        
        BENCHMARK("std::string copying") {
            std::string original(test_string);
            volatile size_t total_length = 0;
            
            for (size_t i = 0; i < iterations; ++i) {
                std::string copy = original;  // Deep copy every time
                total_length += copy.length();
            }
            
            return total_length;
        };
        
        BENCHMARK("COWString copying") {
            COWString original(test_string);
            volatile size_t total_length = 0;
            
            for (size_t i = 0; i < iterations; ++i) {
                COWString copy = original;  // Should be fast (COW)
                total_length += copy.length();
            }
            
            return total_length;
        };
        
        std::cerr << "COWString performance benchmark completed" << std::endl;
    }
    
    SECTION("Memory manager vs malloc/free performance") {
        std::cerr << "\n=== Memory Manager Performance Benchmark ===" << std::endl;
        
        const size_t iterations = 10000;
        const size_t alloc_size = 64;
        
        BENCHMARK("malloc/free") {
            volatile size_t total_allocated = 0;
            
            for (size_t i = 0; i < iterations; ++i) {
                void* ptr = std::malloc(alloc_size);
                if (ptr) {
                    total_allocated += alloc_size;
                    std::free(ptr);
                }
            }
            
            return total_allocated;
        };
        
        BENCHMARK("SimpleMemoryManager") {
            SimpleMemoryManager manager(alloc_size * iterations * 2);  // Large enough pool
            volatile size_t total_allocated = 0;
            
            for (size_t i = 0; i < iterations; ++i) {
                void* ptr = manager.allocate(alloc_size);
                if (ptr) {
                    total_allocated += alloc_size;
                    manager.deallocate(ptr);
                }
            }
            
            return total_allocated;
        };
        
        std::cerr << "Memory manager performance benchmark completed" << std::endl;
    }
}