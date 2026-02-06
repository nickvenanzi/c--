#include "cpp_mastery.hpp"
#include <iostream>
#include <memory>
#include <cstring>

// =============================================================================
// DAY 8: Classes and Objects Fundamentals
// =============================================================================
// Topics Covered:
// - Class definition and member access
// - Constructor and destructor basics
// - this pointer and member functions
// - Object layout in memory
// =============================================================================

// Forward declarations for testing
class SimpleClass;
class BankAccount;
class Rectangle;

TEST_CASE("Class Definition and Member Access", "[classes][members][day8]") {
    
    SECTION("Basic class definition and member access") {
        // Simple class with public and private members
        class SimpleClass {
        public:
            int public_value;
            
            void set_private_value(int value) {
                private_value_ = value;
            }
            
            int get_private_value() const {
                return private_value_;
            }
            
            void display_info() const {
                std::cerr << "Public: " << public_value 
                         << ", Private: " << private_value_ << std::endl;
            }
            
        private:
            int private_value_;
        };
        
        std::cerr << "\n=== Basic Class Usage ===" << std::endl;
        
        SimpleClass obj;
        obj.public_value = 42;           // Direct access to public member
        obj.set_private_value(100);      // Access private member through method
        
        std::cerr << "Public value: " << obj.public_value << std::endl;
        std::cerr << "Private value: " << obj.get_private_value() << std::endl;
        obj.display_info();
        
        REQUIRE(obj.public_value == 42);
        REQUIRE(obj.get_private_value() == 100);
        
        cpp_mastery::Logger::info("Basic class member access demonstrated");
    }
    
    SECTION("Member access specifiers") {
        class AccessDemo {
        public:
            int public_member = 1;
            
            void public_method() {
                std::cerr << "Public method can access all members:" << std::endl;
                std::cerr << "  Public: " << public_member << std::endl;
                std::cerr << "  Protected: " << protected_member << std::endl;
                std::cerr << "  Private: " << private_member << std::endl;
            }
            
        protected:
            int protected_member = 2;
            
        private:
            int private_member = 3;
        };
        
        std::cerr << "\n=== Member Access Specifiers ===" << std::endl;
        
        AccessDemo obj;
        
        // Can access public members
        REQUIRE(obj.public_member == 1);
        obj.public_method();
        
        // Cannot access protected or private members from outside
        // obj.protected_member = 5;  // Compilation error
        // obj.private_member = 6;    // Compilation error
        
        cpp_mastery::Logger::info("Member access specifiers demonstrated");
    }
    
    SECTION("Class vs struct default access") {
        // struct has public default access
        struct PublicByDefault {
            int value = 42;  // Public by default
        };
        
        // class has private default access
        class PrivateByDefault {
            int value = 42;  // Private by default
        public:
            int get_value() const { return value; }
        };
        
        std::cerr << "\n=== Class vs Struct Default Access ===" << std::endl;
        
        PublicByDefault pub_obj;
        PrivateByDefault priv_obj;
        
        // Can access struct member directly
        REQUIRE(pub_obj.value == 42);
        std::cerr << "Struct member (public by default): " << pub_obj.value << std::endl;
        
        // Must use method to access class member
        REQUIRE(priv_obj.get_value() == 42);
        std::cerr << "Class member (private by default): " << priv_obj.get_value() << std::endl;
        
        cpp_mastery::Logger::info("Class vs struct default access demonstrated");
    }
}

TEST_CASE("Constructor and Destructor Basics", "[constructors][destructors][day8]") {
    
    SECTION("Default constructor and destructor") {
        class ResourceDemo {
        public:
            ResourceDemo() {
                std::cerr << "ResourceDemo constructor called" << std::endl;
                data_ = new int(42);
            }
            
            ~ResourceDemo() {
                std::cerr << "ResourceDemo destructor called" << std::endl;
                delete data_;
                data_ = nullptr;
            }
            
            int get_value() const {
                return data_ ? *data_ : 0;
            }
            
        private:
            int* data_;
        };
        
        std::cerr << "\n=== Constructor and Destructor ===" << std::endl;
        
        {
            ResourceDemo obj;  // Constructor called
            REQUIRE(obj.get_value() == 42);
            std::cerr << "Object value: " << obj.get_value() << std::endl;
        }  // Destructor called when obj goes out of scope
        
        cpp_mastery::Logger::info("Constructor and destructor lifecycle demonstrated");
    }
    
    SECTION("Parameterized constructors") {
        class BankAccount {
        public:
            // Default constructor
            BankAccount() : account_number_(0), balance_(0.0) {
                std::cerr << "Default BankAccount created" << std::endl;
            }
            
            // Parameterized constructor
            BankAccount(int account_num, double initial_balance) 
                : account_number_(account_num), balance_(initial_balance) {
                std::cerr << "BankAccount " << account_number_ 
                         << " created with balance $" << balance_ << std::endl;
            }
            
            // Another parameterized constructor
            explicit BankAccount(int account_num) 
                : account_number_(account_num), balance_(0.0) {
                std::cerr << "BankAccount " << account_number_ 
                         << " created with zero balance" << std::endl;
            }
            
            ~BankAccount() {
                std::cerr << "BankAccount " << account_number_ << " destroyed" << std::endl;
            }
            
            void deposit(double amount) {
                if (amount > 0) {
                    balance_ += amount;
                }
            }
            
            double get_balance() const { return balance_; }
            int get_account_number() const { return account_number_; }
            
        private:
            int account_number_;
            double balance_;
        };
        
        std::cerr << "\n=== Parameterized Constructors ===" << std::endl;
        
        BankAccount default_account;                    // Default constructor
        BankAccount savings_account(12345, 1000.0);    // Parameterized constructor
        BankAccount checking_account(67890);           // Single parameter constructor
        
        REQUIRE(default_account.get_account_number() == 0);
        REQUIRE(default_account.get_balance() == 0.0);
        
        REQUIRE(savings_account.get_account_number() == 12345);
        REQUIRE(savings_account.get_balance() == 1000.0);
        
        REQUIRE(checking_account.get_account_number() == 67890);
        REQUIRE(checking_account.get_balance() == 0.0);
        
        // Test deposit functionality
        checking_account.deposit(500.0);
        REQUIRE(checking_account.get_balance() == 500.0);
        
        cpp_mastery::Logger::info("Parameterized constructors demonstrated");
    }
    
    SECTION("Constructor initialization lists") {
        class Rectangle {
        public:
            // Constructor with initialization list (preferred)
            Rectangle(double w, double h) : width_(w), height_(h) {
                std::cerr << "Rectangle created: " << width_ << "x" << height_ << std::endl;
            }
            
            // Constructor with assignment (less efficient)
            Rectangle(double side) {
                width_ = side;   // Assignment, not initialization
                height_ = side;  // Assignment, not initialization
                std::cerr << "Square created: " << side << "x" << side << std::endl;
            }
            
            ~Rectangle() {
                std::cerr << "Rectangle " << width_ << "x" << height_ << " destroyed" << std::endl;
            }
            
            double area() const {
                return width_ * height_;
            }
            
            double perimeter() const {
                return 2 * (width_ + height_);
            }
            
            void display() const {
                std::cerr << "Rectangle: " << width_ << "x" << height_ 
                         << " (Area: " << area() << ", Perimeter: " << perimeter() << ")" << std::endl;
            }
            
        private:
            double width_;
            double height_;
        };
        
        std::cerr << "\n=== Constructor Initialization Lists ===" << std::endl;
        
        Rectangle rect(5.0, 3.0);  // Uses initialization list
        Rectangle square(4.0);     // Uses assignment in constructor body
        
        rect.display();
        square.display();
        
        REQUIRE(rect.area() == 15.0);
        REQUIRE(rect.perimeter() == 16.0);
        REQUIRE(square.area() == 16.0);
        REQUIRE(square.perimeter() == 16.0);
        
        cpp_mastery::Logger::info("Constructor initialization lists demonstrated");
    }
}

TEST_CASE("The 'this' Pointer and Member Functions", "[this][members][day8]") {
    
    SECTION("Understanding the 'this' pointer") {
        class ThisDemo {
        public:
            ThisDemo(int value) : value_(value) {}
            
            void set_value(int value) {
                // Explicit use of 'this' to resolve name conflict
                this->value_ = value;
            }
            
            void set_value_alternative(int value_) {
                // Using 'this' to access member when parameter shadows it
                this->value_ = value_;
            }
            
            ThisDemo& chain_method_1(int val) {
                value_ += val;
                return *this;  // Return reference to current object
            }
            
            ThisDemo& chain_method_2(int val) {
                value_ *= val;
                return *this;  // Enable method chaining
            }
            
            void print_addresses() const {
                std::cerr << "Object address: " << this << std::endl;
                std::cerr << "Member address: " << &value_ << std::endl;
                std::cerr << "this->value_ address: " << &(this->value_) << std::endl;
            }
            
            int get_value() const { return value_; }
            
        private:
            int value_;
        };
        
        std::cerr << "\n=== The 'this' Pointer ===" << std::endl;
        
        ThisDemo obj1(10);
        ThisDemo obj2(20);
        
        std::cerr << "Object 1:" << std::endl;
        obj1.print_addresses();
        
        std::cerr << "Object 2:" << std::endl;
        obj2.print_addresses();
        
        // Demonstrate method chaining using 'this'
        obj1.chain_method_1(5).chain_method_2(2);  // (10 + 5) * 2 = 30
        REQUIRE(obj1.get_value() == 30);
        
        // Test parameter shadowing resolution
        obj2.set_value_alternative(100);
        REQUIRE(obj2.get_value() == 100);
        
        cpp_mastery::Logger::info("'this' pointer usage demonstrated");
    }
    
    SECTION("Const member functions and 'this'") {
        class ConstDemo {
        public:
            ConstDemo(int val) : value_(val), mutable_counter_(0) {}
            
            // Const member function - 'this' is const ConstDemo*
            int get_value() const {
                mutable_counter_++;  // mutable member can be modified
                // value_++;  // Error: cannot modify non-mutable member in const function
                return value_;
            }
            
            // Non-const member function - 'this' is ConstDemo*
            void set_value(int val) {
                value_ = val;
                mutable_counter_++;
            }
            
            int get_counter() const {
                return mutable_counter_;
            }
            
        private:
            int value_;
            mutable int mutable_counter_;  // Can be modified even in const functions
        };
        
        std::cerr << "\n=== Const Member Functions ===" << std::endl;
        
        ConstDemo obj(42);
        const ConstDemo const_obj(100);
        
        // Non-const object can call both const and non-const methods
        obj.set_value(50);
        int val1 = obj.get_value();
        
        // Const object can only call const methods
        int val2 = const_obj.get_value();
        // const_obj.set_value(200);  // Error: cannot call non-const method on const object
        
        REQUIRE(val1 == 50);
        REQUIRE(val2 == 100);
        
        // Mutable counter incremented even in const methods
        REQUIRE(obj.get_counter() > 0);
        REQUIRE(const_obj.get_counter() > 0);
        
        cpp_mastery::Logger::info("Const member functions demonstrated");
    }
}

TEST_CASE("Object Layout in Memory", "[memory][layout][day8]") {
    
    SECTION("Basic object memory layout") {
        class MemoryLayoutDemo {
        public:
            MemoryLayoutDemo(int a, double b, char c) : int_member_(a), double_member_(b), char_member_(c) {}
            
            void print_layout() const {
                std::cerr << "Object size: " << sizeof(*this) << " bytes" << std::endl;
                std::cerr << "Object address: " << this << std::endl;
                std::cerr << "int_member_ address: " << &int_member_ << " (offset: " 
                         << (char*)&int_member_ - (char*)this << ")" << std::endl;
                std::cerr << "double_member_ address: " << &double_member_ << " (offset: " 
                         << (char*)&double_member_ - (char*)this << ")" << std::endl;
                std::cerr << "char_member_ address: " << &char_member_ << " (offset: " 
                         << (char*)&char_member_ - (char*)this << ")" << std::endl;
            }
            
            void print_sizes() const {
                std::cerr << "sizeof(int): " << sizeof(int_member_) << std::endl;
                std::cerr << "sizeof(double): " << sizeof(double_member_) << std::endl;
                std::cerr << "sizeof(char): " << sizeof(char_member_) << std::endl;
            }
            
        private:
            int int_member_;
            double double_member_;
            char char_member_;
        };
        
        std::cerr << "\n=== Object Memory Layout ===" << std::endl;
        
        MemoryLayoutDemo obj(42, 3.14, 'A');
        obj.print_sizes();
        obj.print_layout();
        
        // Object size includes padding for alignment
        size_t expected_min_size = sizeof(int) + sizeof(double) + sizeof(char);
        REQUIRE(sizeof(obj) >= expected_min_size);
        
        cpp_mastery::Logger::info("Object memory layout analyzed");
    }
    
    SECTION("Memory layout with different member orders") {
        // Class with members in order that might cause padding
        class PaddedClass {
        public:
            char c1;     // 1 byte
            double d;    // 8 bytes (likely padded to 8-byte boundary)
            char c2;     // 1 byte
            int i;       // 4 bytes
        };
        
        // Class with members ordered to minimize padding
        class OptimizedClass {
        public:
            double d;    // 8 bytes
            int i;       // 4 bytes
            char c1;     // 1 byte
            char c2;     // 1 byte (packed together)
        };
        
        std::cerr << "\n=== Memory Layout Optimization ===" << std::endl;
        
        std::cerr << "PaddedClass size: " << sizeof(PaddedClass) << " bytes" << std::endl;
        std::cerr << "OptimizedClass size: " << sizeof(OptimizedClass) << " bytes" << std::endl;
        
        // Optimized class should be smaller or equal due to better packing
        REQUIRE(sizeof(OptimizedClass) <= sizeof(PaddedClass));
        
        cpp_mastery::Logger::info("Memory layout optimization demonstrated");
    }
    
    SECTION("Static vs instance members") {
        class StaticDemo {
        public:
            StaticDemo(int val) : instance_value_(val) {
                instance_count_++;
            }
            
            ~StaticDemo() {
                instance_count_--;
            }
            
            static int get_instance_count() {
                return instance_count_;
            }
            
            void print_info() const {
                std::cerr << "Instance value: " << instance_value_ 
                         << ", Total instances: " << instance_count_ << std::endl;
            }
            
            static void print_static_info() {
                std::cerr << "Static method - Total instances: " << instance_count_ << std::endl;
                // std::cerr << instance_value_;  // Error: cannot access instance member from static method
            }
            
        private:
            int instance_value_;           // Each object has its own copy
            static int instance_count_;    // Shared among all objects
        };
        
        std::cerr << "\n=== Static vs Instance Members ===" << std::endl;
        
        REQUIRE(StaticDemo::get_instance_count() == 0);
        StaticDemo::print_static_info();
        
        {
            StaticDemo obj1(10);
            REQUIRE(StaticDemo::get_instance_count() == 1);
            obj1.print_info();
            
            StaticDemo obj2(20);
            REQUIRE(StaticDemo::get_instance_count() == 2);
            obj2.print_info();
            
            StaticDemo obj3(30);
            REQUIRE(StaticDemo::get_instance_count() == 3);
            obj3.print_info();
        }
        
        REQUIRE(StaticDemo::get_instance_count() == 0);
        StaticDemo::print_static_info();
        
        cpp_mastery::Logger::info("Static vs instance members demonstrated");
    }
}

// Static member definition (required outside class)
int StaticDemo::instance_count_ = 0;

TEST_CASE("Class Design Best Practices", "[design][practices][day8]") {
    
    SECTION("Encapsulation and data hiding") {
        class WellEncapsulated {
        public:
            explicit WellEncapsulated(const std::string& name) : name_(name), id_(next_id_++) {}
            
            // Provide controlled access to private data
            const std::string& get_name() const { return name_; }
            int get_id() const { return id_; }
            
            // Validate input before modifying state
            bool set_name(const std::string& name) {
                if (name.empty()) {
                    return false;  // Invalid input
                }
                name_ = name;
                return true;
            }
            
            // Provide meaningful operations, not just getters/setters
            void display() const {
                std::cerr << "Object " << id_ << ": " << name_ << std::endl;
            }
            
        private:
            std::string name_;
            int id_;
            static int next_id_;
        };
        
        std::cerr << "\n=== Encapsulation Best Practices ===" << std::endl;
        
        WellEncapsulated obj1("First Object");
        WellEncapsulated obj2("Second Object");
        
        obj1.display();
        obj2.display();
        
        REQUIRE(obj1.get_id() != obj2.get_id());
        REQUIRE(obj1.get_name() == "First Object");
        
        // Test validation
        REQUIRE(obj1.set_name("New Name") == true);
        REQUIRE(obj1.set_name("") == false);  // Should reject empty name
        
        cpp_mastery::Logger::info("Encapsulation best practices demonstrated");
    }
    
    SECTION("RAII (Resource Acquisition Is Initialization)") {
        class RAIIDemo {
        public:
            explicit RAIIDemo(size_t size) : size_(size) {
                buffer_ = new char[size_];
                std::memset(buffer_, 0, size_);
                std::cerr << "Allocated " << size_ << " bytes" << std::endl;
            }
            
            ~RAIIDemo() {
                delete[] buffer_;
                std::cerr << "Deallocated " << size_ << " bytes" << std::endl;
            }
            
            // Prevent copying for simplicity (we'll cover copy constructors later)
            RAIIDemo(const RAIIDemo&) = delete;
            RAIIDemo& operator=(const RAIIDemo&) = delete;
            
            void write_data(size_t index, char value) {
                if (index < size_) {
                    buffer_[index] = value;
                }
            }
            
            char read_data(size_t index) const {
                return (index < size_) ? buffer_[index] : 0;
            }
            
            size_t size() const { return size_; }
            
        private:
            char* buffer_;
            size_t size_;
        };
        
        std::cerr << "\n=== RAII Pattern ===" << std::endl;
        
        {
            RAIIDemo resource(100);
            resource.write_data(0, 'H');
            resource.write_data(1, 'i');
            
            REQUIRE(resource.read_data(0) == 'H');
            REQUIRE(resource.read_data(1) == 'i');
            REQUIRE(resource.size() == 100);
        }  // Destructor automatically cleans up memory
        
        cpp_mastery::Logger::info("RAII pattern demonstrated");
    }
}

// Static member definition
int WellEncapsulated::next_id_ = 1;

// =============================================================================
// EXERCISES FOR COMPLETION
// =============================================================================

TEST_CASE("Day 8 Exercises - Complete These!", "[exercises][day8]") {
    
    SECTION("Exercise 1: Design a Student class") {
        // TODO: Implement a Student class with the following requirements:
        // 1. Private members: name (string), student_id (int), grades (vector<double>)
        // 2. Constructor that takes name and student_id
        // 3. Methods to add grades, calculate average, and display student info
        // 4. Static member to track total number of students
        // 5. Proper encapsulation with validation

        class Student {
            std::string name;
            int student_id;
            std::vector<double> grades;

            public:
                Student(const std::string& name_, int id_): name(name_), student_id(id_) {}

                void add_grade(double grade) { grades.push_back(grade); }
                double get_average() { 
                    if (grades.empty())
                        return 0.0;
                    return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size(); 
                }
                void display_info() {
                    std::cout << name << ": id = " << student_id << " avg_grade: " << get_average() << '\n';
                }
        }
        
        // Example usage:
        Student s1("Alice", 12345);
        s1.add_grade(85.5);
        s1.add_grade(92.0);
        double avg = s1.get_average();
        s1.display_info();
        
        cpp_mastery::Logger::info("TODO: Implement Student class");
    }
}