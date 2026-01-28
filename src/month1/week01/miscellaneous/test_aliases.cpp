#include "cpp_mastery.hpp"

// Test different ways to create function aliases

// Option 1: Class alias (this definitely works)
using Logger = cpp_mastery::Logger;

// Option 2: Function pointer
auto info_ptr = cpp_mastery::Logger::info;

// Option 3: Function reference  
auto& info_ref = cpp_mastery::Logger::info;

// Option 4: Lambda wrapper
auto info_lambda = [](const std::string& msg) { 
    cpp_mastery::Logger::info(msg); 
};

TEST_CASE("Test function aliases", "[test]") {
    // Test all the different approaches
    Logger::info("Using class alias");
    info_ptr("Using function pointer");
    info_ref("Using function reference");
    info_lambda("Using lambda wrapper");
    
    REQUIRE(true);  // Just to make it a valid test
}