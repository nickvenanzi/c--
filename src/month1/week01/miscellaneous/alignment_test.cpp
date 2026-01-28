#include "cpp_mastery.hpp"
#include <cstdint>

TEST_CASE("Memory Alignment with alignas", "[alignment]") {
    
    SECTION("Default alignment behavior") {
        struct NaturalAlignment {
            char c;      // 1 byte
            int i;       // 4 bytes, naturally aligned to 4-byte boundary
            double d;    // 8 bytes, naturally aligned to 8-byte boundary
        };
        
        NaturalAlignment obj;
        
        std::cerr << "\n=== Natural Alignment ===" << std::endl;
        std::cerr << "struct size: " << sizeof(NaturalAlignment) << " bytes" << std::endl;
        std::cerr << "char address: " << static_cast<void*>(&obj.c) << std::endl;
        std::cerr << "int address: " << static_cast<void*>(&obj.i) << std::endl;
        std::cerr << "double address: " << static_cast<void*>(&obj.d) << std::endl;
        
        // Check natural alignment
        REQUIRE(reinterpret_cast<uintptr_t>(&obj.i) % alignof(int) == 0);
        REQUIRE(reinterpret_cast<uintptr_t>(&obj.d) % alignof(double) == 0);
        
        cpp_mastery::Logger::info("Natural alignment verified");
    }
    
    SECTION("Custom alignment with alignas") {
        struct CustomAlignment {
            alignas(16) char c;      // Force char to 16-byte boundary
            alignas(32) int i;       // Force int to 32-byte boundary  
            alignas(64) double d;    // Force double to 64-byte boundary
        };
        
        CustomAlignment obj;
        
        std::cerr << "\n=== Custom Alignment ===" << std::endl;
        std::cerr << "struct size: " << sizeof(CustomAlignment) << " bytes" << std::endl;
        std::cerr << "char address: " << static_cast<void*>(&obj.c) << std::endl;
        std::cerr << "int address: " << static_cast<void*>(&obj.i) << std::endl;
        std::cerr << "double address: " << static_cast<void*>(&obj.d) << std::endl;
        
        // Verify forced alignment (these MUST be true)
        REQUIRE(reinterpret_cast<uintptr_t>(&obj.c) % 16 == 0);  // 16-byte aligned
        REQUIRE(reinterpret_cast<uintptr_t>(&obj.i) % 32 == 0);  // 32-byte aligned
        REQUIRE(reinterpret_cast<uintptr_t>(&obj.d) % 64 == 0);  // 64-byte aligned
        
        cpp_mastery::Logger::info("Custom alignment requirements satisfied");
    }
    
    SECTION("Cache line alignment") {
        // Align to cache line boundary (typically 64 bytes)
        alignas(64) struct CacheAligned {
            int data[16];  // 64 bytes of data
        };
        
        CacheAligned obj1, obj2;
        
        std::cerr << "\n=== Cache Line Alignment ===" << std::endl;
        std::cerr << "obj1 address: " << static_cast<void*>(&obj1) << std::endl;
        std::cerr << "obj2 address: " << static_cast<void*>(&obj2) << std::endl;
        
        // Both objects must be 64-byte aligned
        REQUIRE(reinterpret_cast<uintptr_t>(&obj1) % 64 == 0);
        REQUIRE(reinterpret_cast<uintptr_t>(&obj2) % 64 == 0);
        
        cpp_mastery::Logger::info("Cache line alignment verified");
    }
    
    SECTION("Variable alignment") {
        // Individual variables with custom alignment
        alignas(16) int aligned_int = 42;
        alignas(32) double aligned_double = 3.14;
        alignas(64) char aligned_char = 'A';
        
        std::cerr << "\n=== Variable Alignment ===" << std::endl;
        std::cerr << "aligned_int address: " << static_cast<void*>(&aligned_int) << std::endl;
        std::cerr << "aligned_double address: " << static_cast<void*>(&aligned_double) << std::endl;
        std::cerr << "aligned_char address: " << static_cast<void*>(&aligned_char) << std::endl;
        
        // Verify alignment requirements
        REQUIRE(reinterpret_cast<uintptr_t>(&aligned_int) % 16 == 0);
        REQUIRE(reinterpret_cast<uintptr_t>(&aligned_double) % 32 == 0);
        REQUIRE(reinterpret_cast<uintptr_t>(&aligned_char) % 64 == 0);
        
        cpp_mastery::Logger::info("Variable alignment requirements satisfied");
    }
    
    SECTION("Array alignment") {
        // Align entire array to specific boundary
        alignas(64) int cache_aligned_array[1000];
        
        std::cerr << "\n=== Array Alignment ===" << std::endl;
        std::cerr << "Array address: " << static_cast<void*>(cache_aligned_array) << std::endl;
        std::cerr << "Array size: " << sizeof(cache_aligned_array) << " bytes" << std::endl;
        
        // Array must start on 64-byte boundary
        REQUIRE(reinterpret_cast<uintptr_t>(cache_aligned_array) % 64 == 0);
        
        cpp_mastery::Logger::info("Array alignment verified");
    }
}

TEST_CASE("Alignment Performance Impact", "[alignment][benchmark]") {
    const size_t iterations = 1000000;
    
    // Misaligned data (worst case)
    struct Misaligned {
        char padding;    // Causes misalignment
        double value;    // Likely misaligned
    };
    
    // Properly aligned data
    struct Aligned {
        alignas(8) double value;  // Guaranteed 8-byte alignment
    };
    
    Misaligned misaligned_array[1000];
    Aligned aligned_array[1000];
    
    // Initialize arrays
    for (size_t i = 0; i < 1000; ++i) {
        misaligned_array[i].value = static_cast<double>(i);
        aligned_array[i].value = static_cast<double>(i);
    }
    
    BENCHMARK("Misaligned access") {
        volatile double sum = 0;
        for (size_t i = 0; i < iterations; ++i) {
            sum += misaligned_array[i % 1000].value;
        }
        return sum;
    };
    
    BENCHMARK("Aligned access") {
        volatile double sum = 0;
        for (size_t i = 0; i < iterations; ++i) {
            sum += aligned_array[i % 1000].value;
        }
        return sum;
    };
}