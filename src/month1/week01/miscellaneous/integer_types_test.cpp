#include "cpp_mastery.hpp"
#include <climits>

TEST_CASE("Integer Type Variations", "[types]") {
    
    SECTION("Type equivalence verification") {
        // Verify that verbose and shorthand forms are identical
        REQUIRE(std::is_same<long int, long>::value);
        REQUIRE(std::is_same<long unsigned int, unsigned long>::value);
        REQUIRE(std::is_same<unsigned long int, unsigned long>::value);
        REQUIRE(std::is_same<signed int, int>::value);
        REQUIRE(std::is_same<signed long int, long>::value);
        
        cpp_mastery::Logger::info("All type equivalences verified");
    }
    
    SECTION("Size analysis") {
        std::cerr << "\n=== Integer Type Sizes ===" << std::endl;
        
        // Basic types
        std::cerr << "char: " << sizeof(char) << " bytes" << std::endl;
        std::cerr << "short: " << sizeof(short) << " bytes" << std::endl;
        std::cerr << "int: " << sizeof(int) << " bytes" << std::endl;
        std::cerr << "long: " << sizeof(long) << " bytes" << std::endl;
        std::cerr << "long long: " << sizeof(long long) << " bytes" << std::endl;
        
        // Verbose forms (should be identical)
        std::cerr << "\n=== Verbose Forms ===" << std::endl;
        std::cerr << "long int: " << sizeof(long int) << " bytes" << std::endl;
        std::cerr << "long unsigned int: " << sizeof(long unsigned int) << " bytes" << std::endl;
        std::cerr << "unsigned long int: " << sizeof(unsigned long int) << " bytes" << std::endl;
        std::cerr << "signed long long int: " << sizeof(signed long long int) << " bytes" << std::endl;
        
        // Verify sizes match
        REQUIRE(sizeof(long) == sizeof(long int));
        REQUIRE(sizeof(unsigned long) == sizeof(long unsigned int));
        REQUIRE(sizeof(unsigned long) == sizeof(unsigned long int));
        REQUIRE(sizeof(long long) == sizeof(signed long long int));
    }
    
    SECTION("Range verification") {
        std::cerr << "\n=== Integer Ranges ===" << std::endl;
        
        std::cerr << "CHAR_MIN: " << CHAR_MIN << ", CHAR_MAX: " << CHAR_MAX << std::endl;
        std::cerr << "SHRT_MIN: " << SHRT_MIN << ", SHRT_MAX: " << SHRT_MAX << std::endl;
        std::cerr << "INT_MIN: " << INT_MIN << ", INT_MAX: " << INT_MAX << std::endl;
        std::cerr << "LONG_MIN: " << LONG_MIN << ", LONG_MAX: " << LONG_MAX << std::endl;
        std::cerr << "LLONG_MIN: " << LLONG_MIN << ", LLONG_MAX: " << LLONG_MAX << std::endl;
        
        // Verify standard guarantees
        REQUIRE(CHAR_MAX >= 127);           // At least 7 bits
        REQUIRE(SHRT_MAX >= 32767);         // At least 15 bits
        REQUIRE(INT_MAX >= 32767);          // At least 15 bits
        REQUIRE(LONG_MAX >= 2147483647L);   // At least 31 bits
        REQUIRE(LLONG_MAX >= 9223372036854775807LL); // At least 63 bits
    }
    
    SECTION("Unusual but valid declarations") {
        // These are all valid C++ syntax
        long int a = 100;
        int long b = 100;                    // Order doesn't matter!
        long unsigned int c = 100;
        unsigned long int d = 100;
        int unsigned long e = 100;           // Any order works
        
        // All should have the same value
        REQUIRE(a == 100);
        REQUIRE(b == 100);
        REQUIRE(c == 100);
        REQUIRE(d == 100);
        REQUIRE(e == 100);
        
        // Types should be equivalent
        REQUIRE(std::is_same<decltype(a), decltype(b)>::value);
        REQUIRE(std::is_same<decltype(c), decltype(d)>::value);
        REQUIRE(std::is_same<decltype(d), decltype(e)>::value);
        
        cpp_mastery::Logger::info("Unusual syntax variations work correctly");
    }
}

TEST_CASE("Fixed-Width Integer Types (C++11)", "[types]") {
    
    SECTION("stdint.h types") {
        std::cerr << "\n=== Fixed-Width Types ===" << std::endl;
        
        std::cerr << "int8_t: " << sizeof(int8_t) << " bytes" << std::endl;
        std::cerr << "int16_t: " << sizeof(int16_t) << " bytes" << std::endl;
        std::cerr << "int32_t: " << sizeof(int32_t) << " bytes" << std::endl;
        std::cerr << "int64_t: " << sizeof(int64_t) << " bytes" << std::endl;
        
        std::cerr << "uint8_t: " << sizeof(uint8_t) << " bytes" << std::endl;
        std::cerr << "uint16_t: " << sizeof(uint16_t) << " bytes" << std::endl;
        std::cerr << "uint32_t: " << sizeof(uint32_t) << " bytes" << std::endl;
        std::cerr << "uint64_t: " << sizeof(uint64_t) << " bytes" << std::endl;
        
        // These have guaranteed sizes
        REQUIRE(sizeof(int8_t) == 1);
        REQUIRE(sizeof(int16_t) == 2);
        REQUIRE(sizeof(int32_t) == 4);
        REQUIRE(sizeof(int64_t) == 8);
        
        cpp_mastery::Logger::info("Fixed-width types have correct sizes");
    }
}