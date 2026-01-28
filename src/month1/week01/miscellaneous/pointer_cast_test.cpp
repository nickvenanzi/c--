#include "cpp_mastery.hpp"
#include <cstdint>

// Test classes for inheritance casting
class Base {
public:
    int base_data = 100;
    virtual ~Base() = default;  // Make it polymorphic
};

class Derived : public Base {
public:
    int derived_data = 200;
};

class Unrelated {
public:
    int unrelated_data = 300;
};

TEST_CASE("Pointer Casting: static_cast vs reinterpret_cast", "[casting][pointers]") {
    
    SECTION("Inheritance hierarchy casting") {
        Derived derived_obj;
        
        // Get the actual memory addresses
        void* derived_addr = &derived_obj;
        void* base_addr = static_cast<Base*>(&derived_obj);
        
        std::cerr << "\n=== Inheritance Casting ===" << std::endl;
        std::cerr << "Derived object address: " << derived_addr << std::endl;
        std::cerr << "Base* (static_cast):    " << base_addr << std::endl;
        
        // static_cast: Safe upcast (Derived* -> Base*)
        Base* base_ptr = static_cast<Base*>(&derived_obj);
        REQUIRE(base_ptr->base_data == 100);
        
        // static_cast: Safe downcast (Base* -> Derived*)
        Derived* derived_ptr = static_cast<Derived*>(base_ptr);
        REQUIRE(derived_ptr->derived_data == 200);
        REQUIRE(derived_ptr->base_data == 100);
        
        cpp_mastery::Logger::info("static_cast handles inheritance correctly");
    }
    
    SECTION("Unrelated type casting - static_cast limitations") {
        Derived derived_obj;
        
        // ❌ This would be a compilation error:
        // Unrelated* bad = static_cast<Unrelated*>(&derived_obj);
        
        // ✅ reinterpret_cast allows this (dangerous!)
        Unrelated* dangerous = reinterpret_cast<Unrelated*>(&derived_obj);
        
        std::cerr << "\n=== Unrelated Type Casting ===" << std::endl;
        std::cerr << "Derived address:     " << static_cast<void*>(&derived_obj) << std::endl;
        std::cerr << "Unrelated* address:  " << static_cast<void*>(dangerous) << std::endl;
        
        // Same memory address, but wrong interpretation!
        REQUIRE(static_cast<void*>(&derived_obj) == static_cast<void*>(dangerous));
        
        // ⚠️ This is undefined behavior - accessing wrong type!
        // dangerous->unrelated_data might crash or return garbage
        
        cpp_mastery::Logger::info("reinterpret_cast allows dangerous conversions");
    }
    
    SECTION("Pointer to integer conversion") {
        int value = 42;
        int* ptr = &value;
        
        // ❌ static_cast cannot convert pointer to integer
        // uintptr_t addr1 = static_cast<uintptr_t>(ptr);  // Compilation error!
        
        // ✅ reinterpret_cast can convert pointer to integer
        uintptr_t addr2 = reinterpret_cast<uintptr_t>(ptr);
        
        std::cerr << "\n=== Pointer to Integer ===" << std::endl;
        std::cerr << "Pointer value: " << ptr << std::endl;
        std::cerr << "As integer:    " << addr2 << std::endl;
        std::cerr << "As hex:        0x" << std::hex << addr2 << std::dec << std::endl;
        
        // Convert back to pointer
        int* restored_ptr = reinterpret_cast<int*>(addr2);
        REQUIRE(restored_ptr == ptr);
        REQUIRE(*restored_ptr == 42);
        
        cpp_mastery::Logger::info("reinterpret_cast handles pointer-integer conversion");
    }
    
    SECTION("Void pointer conversions") {
        int value = 123;
        void* void_ptr = &value;
        
        // ✅ static_cast: Safe conversion from void*
        int* int_ptr1 = static_cast<int*>(void_ptr);
        REQUIRE(*int_ptr1 == 123);
        
        // ✅ reinterpret_cast: Also works but unnecessary here
        int* int_ptr2 = reinterpret_cast<int*>(void_ptr);
        REQUIRE(*int_ptr2 == 123);
        
        // ❌ static_cast: Cannot convert to unrelated type
        // double* bad = static_cast<double*>(void_ptr);  // Compilation error!
        
        // ✅ reinterpret_cast: Allows dangerous conversion
        double* dangerous = reinterpret_cast<double*>(void_ptr);
        // ⚠️ Using dangerous is undefined behavior!
        
        std::cerr << "\n=== Void Pointer Conversions ===" << std::endl;
        std::cerr << "void* address:   " << void_ptr << std::endl;
        std::cerr << "int* address:    " << static_cast<void*>(int_ptr1) << std::endl;
        std::cerr << "double* address: " << static_cast<void*>(dangerous) << std::endl;
        
        cpp_mastery::Logger::info("void* conversions demonstrated");
    }
    
    SECTION("Array reinterpretation") {
        // Treat array of ints as array of bytes
        int int_array[4] = {0x12345678, 0x9ABCDEF0, 0x11223344, 0x55667788};
        
        // ❌ static_cast cannot do this conversion
        // char* bytes1 = static_cast<char*>(int_array);  // Compilation error!
        
        // ✅ reinterpret_cast allows viewing memory as different type
        char* bytes = reinterpret_cast<char*>(int_array);
        
        std::cerr << "\n=== Array Reinterpretation ===" << std::endl;
        std::cerr << "int array address: " << static_cast<void*>(int_array) << std::endl;
        std::cerr << "char* address:     " << static_cast<void*>(bytes) << std::endl;
        std::cerr << "First 4 bytes as hex: ";
        
        for (int i = 0; i < 4; ++i) {
            std::cerr << "0x" << std::hex << (static_cast<unsigned char>(bytes[i]) & 0xFF) << " ";
        }
        std::cerr << std::dec << std::endl;
        
        // Addresses should be the same
        REQUIRE(static_cast<void*>(int_array) == static_cast<void*>(bytes));
        
        cpp_mastery::Logger::info("Array reinterpretation works");
    }
}

TEST_CASE("Multiple Inheritance and Pointer Offsets", "[casting][inheritance]") {
    
    class Base1 {
    public:
        int base1_data = 111;
        virtual ~Base1() = default;
    };
    
    class Base2 {
    public:
        int base2_data = 222;
        virtual ~Base2() = default;
    };
    
    class MultiDerived : public Base1, public Base2 {
    public:
        int derived_data = 333;
    };
    
    SECTION("Multiple inheritance pointer arithmetic") {
        MultiDerived obj;
        
        // Get pointers to different base classes
        MultiDerived* derived_ptr = &obj;
        Base1* base1_ptr = static_cast<Base1*>(&obj);
        Base2* base2_ptr = static_cast<Base2*>(&obj);
        
        std::cerr << "\n=== Multiple Inheritance Addresses ===" << std::endl;
        std::cerr << "MultiDerived*: " << static_cast<void*>(derived_ptr) << std::endl;
        std::cerr << "Base1*:        " << static_cast<void*>(base1_ptr) << std::endl;
        std::cerr << "Base2*:        " << static_cast<void*>(base2_ptr) << std::endl;
        
        // static_cast handles offset calculations automatically
        REQUIRE(base1_ptr->base1_data == 111);
        REQUIRE(base2_ptr->base2_data == 222);
        
        // Base2* will likely have different address due to layout
        uintptr_t derived_addr = reinterpret_cast<uintptr_t>(derived_ptr);
        uintptr_t base1_addr = reinterpret_cast<uintptr_t>(base1_ptr);
        uintptr_t base2_addr = reinterpret_cast<uintptr_t>(base2_ptr);
        
        std::cerr << "Address offset Base1: " << (base1_addr - derived_addr) << " bytes" << std::endl;
        std::cerr << "Address offset Base2: " << (base2_addr - derived_addr) << " bytes" << std::endl;
        
        // reinterpret_cast would NOT handle these offsets correctly
        Base2* wrong_ptr = reinterpret_cast<Base2*>(derived_ptr);
        // ⚠️ wrong_ptr might point to wrong memory location!
        
        cpp_mastery::Logger::info("static_cast handles multiple inheritance offsets");
    }
}