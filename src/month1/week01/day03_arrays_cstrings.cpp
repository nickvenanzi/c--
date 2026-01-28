#include "cpp_mastery.hpp"
#include <cstring>
#include <cstdlib>

// =============================================================================
// DAY 3: Arrays and C-style Strings
// =============================================================================
// Topics Covered:
// - Static arrays and memory layout
// - Dynamic arrays with new/delete
// - C-string manipulation and performance
// - Buffer overflows and safety
// =============================================================================

TEST_CASE("Static Arrays Fundamentals", "[arrays][day3]") {
    
    SECTION("Array declaration and initialization") {
        // Different ways to declare and initialize arrays
        int arr1[5];                           // Uninitialized (contains garbage)
        int arr2[5] = {1, 2, 3, 4, 5};        // Full initialization
        int arr3[5] = {1, 2};                 // Partial initialization (rest are 0)
        int arr4[] = {1, 2, 3, 4, 5, 6};      // Size inferred from initializer
        int arr5[5] = {};                      // Zero initialization
        
        std::cerr << "\n=== Array Initialization ===" << std::endl;
        std::cerr << "arr1 (uninitialized): ";
        for (int i = 0; i < 5; ++i) {
            std::cerr << arr1[i] << " ";  // Garbage values
        }
        std::cerr << std::endl;
        
        std::cerr << "arr2 (full init): ";
        for (int i = 0; i < 5; ++i) {
            std::cerr << arr2[i] << " ";
        }
        std::cerr << std::endl;
        
        std::cerr << "arr3 (partial init): ";
        for (int i = 0; i < 5; ++i) {
            std::cerr << arr3[i] << " ";  // 1 2 0 0 0
        }
        std::cerr << std::endl;
        
        std::cerr << "arr4 size: " << sizeof(arr4) / sizeof(arr4[0]) << std::endl;
        std::cerr << "arr5 (zero init): ";
        for (int i = 0; i < 5; ++i) {
            std::cerr << arr5[i] << " ";  // All zeros
        }
        std::cerr << std::endl;
        
        // Verify initialization behavior
        REQUIRE(arr2[0] == 1);
        REQUIRE(arr2[4] == 5);
        REQUIRE(arr3[2] == 0);  // Unspecified elements are zero
        REQUIRE(arr5[0] == 0);  // Zero initialization
        
        cpp_mastery::Logger::info("Array initialization patterns verified");
    }
    
    SECTION("Array memory layout and addressing") {
        int arr[5] = {10, 20, 30, 40, 50};
        
        std::cerr << "\n=== Array Memory Layout ===" << std::endl;
        std::cerr << "Array name (arr): " << arr << std::endl;
        std::cerr << "Address of arr[0]: " << &arr[0] << std::endl;
        std::cerr << "Address of arr: " << &arr << std::endl;
        
        for (int i = 0; i < 5; ++i) {
            std::cerr << "arr[" << i << "] = " << arr[i] 
                     << " at address " << &arr[i] 
                     << " (offset: " << (reinterpret_cast<char*>(&arr[i]) - reinterpret_cast<char*>(arr)) 
                     << " bytes)" << std::endl;
        }
        
        // Verify contiguous memory layout
        REQUIRE(arr == &arr[0]);  // Array name is address of first element
        REQUIRE(&arr[1] - &arr[0] == 1);  // Elements are adjacent
        REQUIRE(reinterpret_cast<char*>(&arr[1]) - reinterpret_cast<char*>(&arr[0]) == sizeof(int));
        
        cpp_mastery::Logger::info("Array memory layout verified");
    }
    
    SECTION("Array bounds and safety") {
        int arr[5] = {1, 2, 3, 4, 5};
        
        std::cerr << "\n=== Array Bounds ===" << std::endl;
        
        // Safe access
        for (int i = 0; i < 5; ++i) {
            std::cerr << "Safe access arr[" << i << "] = " << arr[i] << std::endl;
            REQUIRE(arr[i] == i + 1);
        }
        
        // Demonstrate bounds checking (carefully!)
        std::cerr << "Array size: " << sizeof(arr) / sizeof(arr[0]) << std::endl;
        
        // TODO: Implement safe array access function
        auto safe_array_access = [](int* array, size_t size, int index) -> int {
            if (index < static_cast<int>(size)) {
                return array[index];
            }
            return 0;
        };
        REQUIRE(safe_array_access(arr, 5, 2) == 3);
        REQUIRE(safe_array_access(arr, 5, 4) == 5);
        REQUIRE(safe_array_access(arr, 5, 5) == 0);
        // Test safe access
        int result = safe_array_access(arr, 5, 2);
        std::cerr << "Safe access result: " << result << std::endl;
        
        cpp_mastery::Logger::info("Array bounds safety demonstrated");
    }
}

TEST_CASE("Dynamic Arrays", "[arrays][dynamic][day3]") {
    
    SECTION("Dynamic array allocation with new/delete") {
        const size_t size = 10;
        
        // Allocate dynamic array
        int* dynamic_arr = new int[size];
        
        std::cerr << "\n=== Dynamic Array Allocation ===" << std::endl;
        std::cerr << "Dynamic array address: " << dynamic_arr << std::endl;
        std::cerr << "Array size: " << size << " elements" << std::endl;
        
        // Initialize array
        for (size_t i = 0; i < size; ++i) {
            dynamic_arr[i] = static_cast<int>(i * i);
        }
        
        // Verify initialization
        for (size_t i = 0; i < size; ++i) {
            std::cerr << "dynamic_arr[" << i << "] = " << dynamic_arr[i] << std::endl;
            REQUIRE(dynamic_arr[i] == static_cast<int>(i * i));
        }
        
        // Clean up
        delete[] dynamic_arr;  // Important: use delete[] for arrays
        dynamic_arr = nullptr;  // Prevent accidental reuse
        
        cpp_mastery::Logger::info("Dynamic array allocation/deallocation verified");
    }
    
    SECTION("Dynamic array resizing") {
        size_t initial_size = 5;
        size_t new_size = 10;
        
        // Initial allocation
        int* arr = new int[initial_size];
        for (size_t i = 0; i < initial_size; ++i) {
            arr[i] = static_cast<int>(i + 1);
        }
        
        std::cerr << "\n=== Dynamic Array Resizing ===" << std::endl;
        std::cerr << "Original array: ";
        for (size_t i = 0; i < initial_size; ++i) {
            std::cerr << arr[i] << " ";
        }
        std::cerr << std::endl;
        
        // Resize array (manual implementation)
        int* new_arr = new int[new_size];
        
        // Copy old data
        std::memcpy(new_arr, arr, initial_size * sizeof(int));
        
        // Initialize new elements
        for (size_t i = initial_size; i < new_size; ++i) {
            new_arr[i] = 0;
        }
        
        // Clean up old array
        delete[] arr;
        arr = new_arr;
        
        std::cerr << "Resized array: ";
        for (size_t i = 0; i < new_size; ++i) {
            std::cerr << arr[i] << " ";
        }
        std::cerr << std::endl;
        
        // Verify resize
        REQUIRE(arr[0] == 1);  // Old data preserved
        REQUIRE(arr[4] == 5);  // Old data preserved
        REQUIRE(arr[5] == 0);  // New elements initialized
        
        delete[] arr;
        
        cpp_mastery::Logger::info("Dynamic array resizing demonstrated");
    }
}

TEST_CASE("C-style Strings", "[strings][cstring][day3]") {
    
    SECTION("C-string basics and null termination") {
        // Different ways to create C-strings
        char str1[] = "Hello";                    // Array with automatic sizing
        char str2[10] = "World";                  // Fixed size array
        char str3[] = {'H', 'i', '\0'};          // Manual character array
        const char* str4 = "Constant string";    // String literal (read-only)
        
        std::cerr << "\n=== C-string Basics ===" << std::endl;
        std::cerr << "str1: \"" << str1 << "\" (length: " << strlen(str1) << ")" << std::endl;
        std::cerr << "str2: \"" << str2 << "\" (length: " << strlen(str2) << ")" << std::endl;
        std::cerr << "str3: \"" << str3 << "\" (length: " << strlen(str3) << ")" << std::endl;
        std::cerr << "str4: \"" << str4 << "\" (length: " << strlen(str4) << ")" << std::endl;
        
        // Memory layout analysis
        std::cerr << "str1 size in memory: " << sizeof(str1) << " bytes" << std::endl;
        std::cerr << "str2 size in memory: " << sizeof(str2) << " bytes" << std::endl;
        
        // Verify null termination
        REQUIRE(str1[5] == '\0');  // Null terminator
        REQUIRE(strlen(str1) == 5);  // Length doesn't include null terminator
        REQUIRE(sizeof(str1) == 6);  // Size includes null terminator
        
        cpp_mastery::Logger::info("C-string basics verified");
    }
    
    SECTION("String manipulation functions") {
        char dest[50];
        const char* src = "Hello, World!";
        
        std::cerr << "\n=== String Manipulation ===" << std::endl;
        
        // String copy
        strcpy(dest, src);
        std::cerr << "After strcpy: \"" << dest << "\"" << std::endl;
        REQUIRE(strcmp(dest, src) == 0);
        
        // String concatenation
        strcat(dest, " How are you?");
        std::cerr << "After strcat: \"" << dest << "\"" << std::endl;
        
        // String length
        size_t len = strlen(dest);
        std::cerr << "String length: " << len << std::endl;
        
        // String comparison
        const char* other = "Hello, World! How are you?";
        int cmp_result = strcmp(dest, other);
        std::cerr << "String comparison result: " << cmp_result << std::endl;
        REQUIRE(cmp_result == 0);  // Strings should be equal
        
        cpp_mastery::Logger::info("String manipulation functions verified");
    }
    
    SECTION("Safe string functions") {
        char dest[20];  // Deliberately small buffer
        const char* src = "This is a very long string that might overflow";
        
        std::cerr << "\n=== Safe String Functions ===" << std::endl;
        
        // Safe copy with size limit
        strncpy(dest, src, sizeof(dest) - 1);
        dest[sizeof(dest) - 1] = '\0';  // Ensure null termination
        
        std::cerr << "Safe copy result: \"" << dest << "\"" << std::endl;
        std::cerr << "Destination buffer size: " << sizeof(dest) << std::endl;
        std::cerr << "Copied string length: " << strlen(dest) << std::endl;
        
        REQUIRE(strlen(dest) < sizeof(dest));  // No overflow
        
        cpp_mastery::Logger::info("Safe string functions demonstrated");
    }
    
    // TODO: Implement custom string functions
    SECTION("Custom string function implementations") {
        
        // TODO: Implement your own strlen
        auto my_strlen = [](const char* str) -> size_t {
            int index = 0;
            while (str[index] != '\0') { index++; }
            return static_cast<size_t>(index);  // Replace with your implementation
        };
        
        // TODO: Implement your own strcpy
        auto my_strcpy = [](char* dest, const char* src) -> char* {
            int index = 0;
            while (src[index] != '\0') {
                dest[index] = src[index];
                index++;
            }
            dest[index] = '\0';
            return dest;  // Replace with your implementation
        };
        
        // TODO: Implement your own strcmp
        auto my_strcmp = [](const char* str1, const char* str2) -> int {
            // TODO: Compare strings character by character
            int index = 0;
            while (str1[index] == str2[index]) {
                if (str1[index] == '\0')
                    break;
                index++;
            }
            return str1[index] - str2[index];  // Replace with your implementation
        };
        
        // Test your implementations
        const char* test_str = "Hello";
        char buffer[20];
        
        // TODO: Uncomment these tests when you implement the functions
        size_t len = my_strlen(test_str);
        REQUIRE(len == 5);
        
        my_strcpy(buffer, test_str);
        REQUIRE(strcmp(buffer, test_str) == 0);
        
        int cmp = my_strcmp("abc", "abc");
        REQUIRE(cmp == 0);
        std::cout << buffer << "\n";
        std::cout << test_str << "\n";
        cpp_mastery::Logger::info("TODO: Complete custom string function implementations");
    }
}

TEST_CASE("Array and String Performance", "[performance][day3]") {
    const size_t size = 100000;
    
    SECTION("Array access patterns") {
        auto data = cpp_mastery::generate_sequential_data<int>(size, 1);
        
        BENCHMARK("Sequential access") {
            volatile long sum = 0;
            for (size_t i = 0; i < size; ++i) {
                sum += data[i];  // Cache-friendly
            }
            return sum;
        };
        
        BENCHMARK("Random access") {
            volatile long sum = 0;
            for (size_t i = 0; i < size; ++i) {
                size_t idx = (i * 7919) % size;  // Pseudo-random pattern
                sum += data[idx];  // Cache-unfriendly
            }
            return sum;
        };
        
        BENCHMARK("Stride access (every 8th element)") {
            volatile long sum = 0;
            for (size_t i = 0; i < 8*size; i += 8) {
                sum += data[i%size];  // Strided access pattern
            }
            return sum;
        };
    }

    SECTION("String operation performance") {
        const char* source = "This is a test string for performance measurement";
        const size_t str_len = strlen(source);
        char* buffer = new char[str_len + 1];
        const size_t iterations = 100;
        
        BENCHMARK("strcpy performance") {
            int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                strcpy(buffer, source);
                sum += static_cast<int>(buffer[i%16]);
            }
            return sum;
        };
        
        BENCHMARK("memcpy performance") {
            int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                memcpy(buffer, source, str_len + 1);
                sum += static_cast<int>(buffer[i%16]);
            }
            return sum;
        };
        
        BENCHMARK("Manual copy loop") {
            int sum = 0;
            for (size_t i = 0; i < iterations; ++i) {
                for (size_t j = 0; j <= str_len; ++j) {
                    buffer[j] = source[j];
                }
                sum += static_cast<int>(buffer[i%16]);
            }
            return sum;
        };
        
        delete[] buffer;
    }
}

TEST_CASE("Buffer Safety and Security", "[security][day3]") {
    
    SECTION("Buffer overflow demonstration (safe)") {
        char small_buffer[10];
        const char* large_string = "This string is definitely too long for the buffer";
        
        std::cerr << "\n=== Buffer Overflow Prevention ===" << std::endl;
        std::cerr << "Buffer size: " << sizeof(small_buffer) << " bytes" << std::endl;
        std::cerr << "String length: " << strlen(large_string) << " bytes" << std::endl;
        
        // Safe copy using strncpy
        strncpy(small_buffer, large_string, sizeof(small_buffer) - 1);
        small_buffer[sizeof(small_buffer) - 1] = '\0';  // Ensure null termination
        
        std::cerr << "Safe copy result: \"" << small_buffer << "\"" << std::endl;
        std::cerr << "Actual length: " << strlen(small_buffer) << std::endl;
        
        REQUIRE(strlen(small_buffer) < sizeof(small_buffer));
        
        cpp_mastery::Logger::info("Buffer overflow prevention demonstrated");
    }
    
    SECTION("Array bounds checking") {
        const size_t array_size = 10;
        int arr[array_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        // TODO: Implement safe array access with bounds checking
        auto safe_get = [&](int index) -> int {
            if (index >= 0 && index < static_cast<int>(array_size)) {
                return arr[index];
            }
            throw std::out_of_range("Index out of bounds");
        };
        
        auto safe_set = [&](int index, int value) -> bool {
            // TODO: Check bounds before setting
            if (index >= 0 && index < static_cast<int>(array_size)) {
                arr[index] = value;
                return true;
            }
            return false;
        };
        
        // Test safe access
        std::cerr << "\n=== Safe Array Access ===" << std::endl;
        for (int i = 0; i < static_cast<int>(array_size); ++i) {
            int value = safe_get(i);
            std::cerr << "arr[" << i << "] = " << value << std::endl;
            REQUIRE(value == i);
        }
        
        // TODO: Test out-of-bounds access safely
        bool success = safe_set(15, 999);  // Should fail
        REQUIRE(!success);
        
        cpp_mastery::Logger::info("Array bounds checking implemented");
    }
    
    // TODO: Implement string validation functions
    SECTION("String validation and sanitization") {
        
        // TODO: Implement input validation
        auto is_valid_string = [](const char* str, size_t max_length) -> bool {
            // TODO: Check for null pointer, reasonable length, valid characters
            if (!str) return false;
            size_t len = strlen(str);
            return len <= max_length && len > 0;
        };
        
        // TODO: Implement string sanitization
        auto sanitize_string = [](char* str) -> void {
            // TODO: Remove or replace dangerous characters
            // while (*str) {
            //     if (*str == '<' || *str == '>' || *str == '&') {
            //         *str = '_';  // Replace with safe character
            //     }
            //     str++;
            // }
        };
        
        // Test cases
        char test_str[] = "Hello <script>alert('xss')</script> World";
        std::cerr << "\n=== String Sanitization ===" << std::endl;
        std::cerr << "Original: " << test_str << std::endl;
        
        sanitize_string(test_str);
        std::cerr << "Sanitized: " << test_str << std::endl;
        
        cpp_mastery::Logger::info("TODO: Complete string validation and sanitization");
    }
}

// =============================================================================
// EXERCISES FOR COMPLETION
// =============================================================================

TEST_CASE("Day 3 Exercises - Complete These!", "[exercises][day3]") {
    
    SECTION("Exercise 1: Matrix operations") {
        const size_t rows = 3, cols = 3;
        
        // TODO: Implement 2D array allocation and operations
        // 1. Allocate a 2D array dynamically
        // 2. Initialize with values
        // 3. Implement matrix addition
        // 4. Implement matrix multiplication
        // 5. Clean up memory properly
        
        int** matrix1 = new int*[rows];
        for (size_t i = 0; i < rows; ++i) {
            matrix1[i] = new int[cols];
        }

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix1[i][j] = matrix1[i][j] + matrix1[i][j];
            }
        }

        for (size_t i = 0; i < rows; i++) {
            delete[] matrix1[i];
        }
        delete[] matrix1;
        
        cpp_mastery::Logger::info("TODO: Complete matrix operations");
    }
    
    SECTION("Exercise 2: String processing utilities") {
        const char* input = "  Hello,   World!  How are you?  ";
        
        // TODO: Implement string processing functions
        // 1. Trim whitespace from beginning and end
        // 2. Count words in string
        // 3. Reverse string in-place
        // 4. Find and replace substring
        
        auto trim_string = [](char* str) -> char* {
            // TODO: Remove leading and trailing whitespace
            int end_idx = 0, start_idx = 0;
            while (str[start_idx] != '\0') {
                if (str[start_idx] != ' ') {
                    str[end_idx] = str[start_idx];
                    end_idx++;
                }
                start_idx++;
            }
            str[end_idx] = '\0';
            return str;  // Replace with implementation
        };
        
        auto count_words = [](const char* str) -> int {
            // TODO: Count words separated by whitespace
            return 0;  // Replace with implementation
        };
        
        auto reverse_string = [](char* str) -> void {
            // TODO: Reverse string in-place
        };
        
        // Test your implementations
        char test_buffer[100];
        strcpy(test_buffer, input);
        
        // TODO: Test your functions
        
        cpp_mastery::Logger::info("TODO: Complete string processing utilities");
    }
    
    SECTION("Exercise 3: Memory-efficient string class") {
        // TODO: Implement a basic string class with:
        // 1. Dynamic memory management
        // 2. Copy constructor and assignment operator
        // 3. Concatenation operator
        // 4. Comparison operators
        // 5. Substring extraction
        // 6. Memory optimization (small string optimization?)
        
        class SimpleString {
        private:
            char* data_;
            size_t length_;
            size_t capacity_;
            
        public:
            // TODO: Implement all necessary methods
            SimpleString() : data_(nullptr), length_(0), capacity_(0) {}
            
            explicit SimpleString(const char* str) {
                length_ = strlen(str);
                capacity_ = length_;
                data_ = new char[length_+1];
                strcpy(data_, str);
            }
            
            ~SimpleString() {
                delete[] data_;
            }
            
            SimpleString(const SimpleString& ss) {
                length_ = ss.length_;
                capacity_ = ss.capacity_;
                data_ = new char[capacity_+1];
                if (ss.data_ == nullptr) {
                    length_ = 0;
                    data_[0] = '\0';
                } else {
                    strcpy(data_, ss.data_);
                }
            }

            SimpleString& operator=(const SimpleString& ss) {
                if (&ss == this) {
                    return *this;
                }
                length_ = ss.length_;
                if (capacity_ < length_) {
                    capacity_ = ss.capacity_;
                    delete[] data_;
                    data_ = new char[capacity_+1];
                }
                strcpy(data_, ss.data_);
                return *this;
            }

            SimpleString& operator+=(const SimpleString& ss) {
                length_ += ss.length_;
                if (capacity_ < length_) {
                    capacity_ += ss.capacity_;
                    data_ = static_cast<char*>(realloc(static_cast<void*>(data_), capacity_+1));
                }
                strcpy(data_ + length_ - ss.length_, ss.data_);
                return *this;
            }

            SimpleString& operator+=(const char* str) {
                size_t add_len = strlen(str);
                length_ += add_len;
                if (capacity_ < length_) {
                    capacity_ += add_len;
                    data_ = static_cast<char*>(realloc(static_cast<void*>(data_), capacity_+1));
                }
                strcpy(data_ + length_ - add_len, str);
                return *this;
            }

            size_t getLength() {
                return length_;
            }

            size_t getCapacity() {
                return capacity_;
            }

            char* getData() {
                return data_;
            }
        };
        
        SimpleString ss{};
        REQUIRE(ss.getLength() == 0);
        ss += "Hello";
        std::cout << ss.getData() << '\n';
        REQUIRE(ss.getLength() == 5);
        SimpleString ss2(" my friend");
        REQUIRE(ss2.getLength() == 10);
        ss += ss2;
        REQUIRE(ss.getLength() == 15);
        std::cout << ss.getData() << '\n';
        SimpleString ss4 = ss;
        REQUIRE(ss4.getLength() == 15);
        ss4 += "...Revised!";
        ss = ss4;
        REQUIRE(ss.getLength() == 26);
        std::cout << ss.getData() << '\n';
        
        cpp_mastery::Logger::info("TODO: Complete memory-efficient string class");
    }
}