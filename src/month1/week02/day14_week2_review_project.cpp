#include "cpp_mastery.hpp"
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <chrono>

// =============================================================================
// DAY 14: Week 2 Review - Object-Oriented Programming Projects
// =============================================================================
// PROJECT 1: Smart Vector - Custom container with RAII and operator overloading
// PROJECT 4: Template Matrix - Generic matrix class with advanced features
//
// You will implement all classes according to the specifications below.
// Comprehensive test suites verify correctness and performance.
// =============================================================================

// =============================================================================
// PROJECT 1 SPECIFICATION: SmartVector Class
// =============================================================================

template<typename T>
class SmartVector {
public:
    // Constructors and destructor
    SmartVector() : capacity_(64), size_(0) {
        data_ = static_cast<T*>(malloc(64*sizeof(T)));
    }

    explicit SmartVector(size_t initial_capacity) : capacity_(initial_capacity), size_(0) {
        data_ = static_cast<T*>(malloc(initial_capacity*sizeof(T)));
    }

    SmartVector(size_t count, const T& value) : capacity_(count), size_(count) {
        data_ = static_cast<T*>(malloc(count*sizeof(T)));
        std::fill_n(data_, count, value);
    }

    SmartVector(std::initializer_list<T> init) : capacity_(init.size()), size_(init.size()) {
        data_ = static_cast<T*>(malloc(init.size()*sizeof(T)));
        std::copy(init.begin(), init.end(), data_);
    }

    SmartVector(const SmartVector& other) : capacity_(other.capacity_), size_(other.size_) {
        data_ = static_cast<T*>(malloc(capacity_*sizeof(T)));
        memcpy(data_, other.data_, size_*sizeof(T));
    }

    SmartVector& operator=(const SmartVector& other) {
        if (&other == this)
            return *this;
        size_ = other.size_;
        if (capacity_ < size_) { // need more space
            capacity_ = other.capacity_;
            free(data_);
            data_ = static_cast<T*>(malloc(capacity_*sizeof(T)));
        }
        memcpy(data_, other.data_, size_*sizeof(T));
        return *this;
    }

    ~SmartVector() {
        free(data_);
    }
    
    // Element access
    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& at(size_t index) {
        if (index < size_) {
            return data_[index];
        }
        throw std::out_of_range("bad index");
    }

    const T& at(size_t index) const {
        if (index < size_) {
            return data_[index];
        }
        throw std::out_of_range("bad index");
    }

    T& front() {
        return data_[0];
    }

    const T& front() const {
        return data_[0];
    }

    T& back() {
        return data_[size_ - 1];
    }

    const T& back() const {
        return data_[size_ - 1];
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }
    
    // Capacity
    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            data_ = static_cast<T*>(realloc(data_, sizeof(T)*new_capacity));
            capacity_ = new_capacity;
        }
    }

    void shrink_to_fit() {
        if (capacity_ > size_) {
            data_ = static_cast<T*>(realloc(data_, sizeof(T)*size_));
            capacity_ = size_;
        }
    }
    
    // Modifiers
    void clear() {
        for (size_t i = 0; i < size_; i++) {
            data_[i].~T();
        }
        size_ = 0;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            data_ = static_cast<T*>(realloc(data_, sizeof(T)*capacity_));
        }
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ > 0)
            data_[size_--].~T();
    }

    void resize(size_t new_size) {
        resize(new_size, T());
    }

    void resize(size_t new_size, const T& value) {
        if (new_size < size_) { // destroy last items to shrink
            for (size_t i = new_size; i < size_; i++) {
                data_[i].~T();
            }
        } else {
            reserve(new_size); // realloc if needed
            for (size_t i = size_; i < new_size; i++) {
                data_[i] = value;
            }
        }
        size_ = new_size;
    }
    
    // Operators
    SmartVector operator+(const SmartVector& other) const {
        SmartVector sv = SmartVector(size_ + other.size_);
        memcpy(sv.data_, data_, sizeof(T)*size_);
        memcpy(sv.data_ + size_, other.data_, sizeof(T)*other.size_);
        sv.size_ = sv.capacity_;
        return sv;
    }

    SmartVector& operator+=(const SmartVector& other) {
        reserve(size_ + other.size_);
        memcpy(data_ + size_, other.data_, sizeof(T)*other.size_);
        size_ += other.size_;
        return *this;
    }

    bool operator==(const SmartVector& other) const {
        if (size_ != other.size_)
            return false;
        for (size_t i = 0; i < size_; i++) {
            if (data_[i] != other.data_[i])
                return false;
        }
        return true;
    }

    bool operator!=(const SmartVector& other) const {
        if (size_ != other.size_)
            return true;
        for (size_t i = 0; i < size_; i++) {
            if (data_[i] != other.data_[i])
                return true;
        }
        return false;
    }
    
    // Iterator support (basic)
    T* begin() {
        return data_;
    }

    const T* begin() const {
        return data_;
    }
    
    T* end() {
        return data_ + size_;
    }

    const T* end() const {
        return data_ + size_;
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

// // =============================================================================
// // PROJECT 4 SPECIFICATION: Template Matrix Class
// // =============================================================================

// template<typename T>
// class Matrix {
// public:
//     // Constructors and destructor
//     Matrix();
//     Matrix(size_t rows, size_t cols);
//     Matrix(size_t rows, size_t cols, const T& initial_value);
//     Matrix(std::initializer_list<std::initializer_list<T>> init);
//     Matrix(const Matrix& other);
//     Matrix& operator=(const Matrix& other);
//     ~Matrix();
    
//     // Element access
//     T& operator()(size_t row, size_t col);
//     const T& operator()(size_t row, size_t col) const;
//     T& at(size_t row, size_t col);
//     const T& at(size_t row, size_t col) const;
    
//     // Capacity
//     size_t rows() const;
//     size_t cols() const;
//     size_t size() const;
//     bool empty() const;
    
//     // Modifiers
//     void resize(size_t new_rows, size_t new_cols);
//     void resize(size_t new_rows, size_t new_cols, const T& value);
//     void fill(const T& value);
//     void clear();
    
//     // Matrix operations
//     Matrix operator+(const Matrix& other) const;
//     Matrix operator-(const Matrix& other) const;
//     Matrix operator*(const Matrix& other) const;
//     Matrix& operator+=(const Matrix& other);
//     Matrix& operator-=(const Matrix& other);
//     Matrix& operator*=(const T& scalar);
    
//     // Comparison operators
//     bool operator==(const Matrix& other) const;
//     bool operator!=(const Matrix& other) const;
    
//     // Matrix-specific operations
//     Matrix transpose() const;
//     T determinant() const;  // For square matrices
//     Matrix inverse() const; // For square matrices
//     bool is_square() const;
    
//     // Utility functions
//     void print() const;
//     std::string to_string() const;

// private:
//     T** data_;
//     size_t rows_;
//     size_t cols_;
    
//     void allocate_memory();
//     void deallocate_memory();
//     void copy_from(const Matrix& other);
// };

// =============================================================================
// TEST SUITE FOR SmartVector
// =============================================================================

TEST_CASE("SmartVector Implementation Tests", "[smartvector][project1][day14]") {
    
    SECTION("Construction and basic operations") {
        std::cerr << "\n=== SmartVector Construction ===" << std::endl;
        
        // Default constructor
        SmartVector<int> vec1;
        REQUIRE(vec1.empty());
        REQUIRE(vec1.size() == 0);
        REQUIRE(vec1.capacity() >= 0);
        
        // Constructor with capacity
        SmartVector<int> vec2(10);
        REQUIRE(vec2.empty());
        REQUIRE(vec2.capacity() >= 10);
        
        // Constructor with count and value
        SmartVector<int> vec3(5, 42);
        REQUIRE(vec3.size() == 5);
        REQUIRE(vec3[0] == 42);
        REQUIRE(vec3[4] == 42);
        
        // Initializer list constructor
        SmartVector<int> vec4{1, 2, 3, 4, 5};
        REQUIRE(vec4.size() == 5);
        REQUIRE(vec4[0] == 1);
        REQUIRE(vec4[4] == 5);
        
        std::cerr << "SmartVector construction tests passed" << std::endl;
    }
    
    SECTION("Element access and modification") {
        SmartVector<std::string> vec{"hello", "world", "test"};
        
        // Operator[] access
        REQUIRE(vec[0] == "hello");
        REQUIRE(vec[1] == "world");
        REQUIRE(vec[2] == "test");
        
        // Modify elements
        vec[1] = "C++";
        REQUIRE(vec[1] == "C++");
        
        // at() with bounds checking
        REQUIRE(vec.at(0) == "hello");
        REQUIRE_THROWS_AS(vec.at(10), std::out_of_range);
        
        // front() and back()
        REQUIRE(vec.front() == "hello");
        REQUIRE(vec.back() == "test");
        
        std::cerr << "Element access tests passed" << std::endl;
    }
    
    SECTION("Dynamic growth and capacity management") {
        SmartVector<int> vec;
        
        // Test push_back and automatic growth
        for (int i = 0; i < 100; ++i) {
            vec.push_back(i);
        }
        
        REQUIRE(vec.size() == 100);
        REQUIRE(vec.capacity() >= 100);
        
        // Test reserve
        vec.reserve(200);
        REQUIRE(vec.capacity() >= 200);
        
        // Test shrink_to_fit
        vec.shrink_to_fit();
        REQUIRE(vec.capacity() >= vec.size());
        
        std::cerr << "Capacity management tests passed" << std::endl;
    }
    
    SECTION("Copy semantics and assignment") {
        SmartVector<int> original{1, 2, 3, 4, 5};
        
        // Copy constructor
        SmartVector<int> copy1(original);
        REQUIRE(copy1.size() == original.size());
        REQUIRE(copy1 == original);
        
        // Assignment operator
        SmartVector<int> copy2;
        copy2 = original;
        REQUIRE(copy2 == original);
        
        // Modify original, copies should be unaffected
        original[0] = 999;
        REQUIRE(copy1[0] == 1);
        REQUIRE(copy2[0] == 1);
        
        std::cerr << "Copy semantics tests passed" << std::endl;
    }
    
    SECTION("Operator overloading") {
        SmartVector<int> vec1{1, 2, 3};
        SmartVector<int> vec2{4, 5, 6};
        
        // Addition operator
        SmartVector<int> result = vec1 + vec2;
        REQUIRE(result.size() == 6);
        REQUIRE(result[0] == 1);
        REQUIRE(result[3] == 4);
        
        // += operator
        vec1 += vec2;
        REQUIRE(vec1.size() == 6);
        REQUIRE(vec1 == result);
        
        std::cerr << "Operator overloading tests passed" << std::endl;
    }
    
    SECTION("Performance benchmarks") {
        const size_t test_size = 100000;
        
        BENCHMARK("SmartVector push_back performance") {
            SmartVector<int> vec;
            for (size_t i = 0; i < test_size; ++i) {
                vec.push_back(static_cast<int>(i));
            }
            return vec.size();
        };
        
        BENCHMARK("std::vector push_back performance") {
            std::vector<int> vec;
            for (size_t i = 0; i < test_size; ++i) {
                vec.push_back(static_cast<int>(i));
            }
            return vec.size();
        };
        
        BENCHMARK("SmartVector element access") {
            SmartVector<int> vec(test_size, 42);
            volatile int sum = 0;
            for (size_t i = 0; i < test_size; ++i) {
                sum += vec[i];
            }
            return sum;
        };
        
        std::cerr << "Performance benchmarks completed" << std::endl;
    }
}

// // =============================================================================
// // TEST SUITE FOR Template Matrix
// // =============================================================================

// TEST_CASE("Matrix Implementation Tests", "[matrix][project4][day14]") {
    
//     SECTION("Matrix construction and basic operations") {
//         std::cerr << "\n=== Matrix Tests ===" << std::endl;
        
//         // Default constructor
//         Matrix<int> mat1;
//         REQUIRE(mat1.empty());
//         REQUIRE(mat1.rows() == 0);
//         REQUIRE(mat1.cols() == 0);
        
//         // Dimension constructor
//         Matrix<double> mat2(3, 4);
//         REQUIRE(mat2.rows() == 3);
//         REQUIRE(mat2.cols() == 4);
//         REQUIRE_FALSE(mat2.empty());
        
//         // Constructor with initial value
//         Matrix<int> mat3(2, 2, 42);
//         REQUIRE(mat3(0, 0) == 42);
//         REQUIRE(mat3(1, 1) == 42);
        
//         // Initializer list constructor
//         Matrix<int> mat4{{1, 2, 3}, {4, 5, 6}};
//         REQUIRE(mat4.rows() == 2);
//         REQUIRE(mat4.cols() == 3);
//         REQUIRE(mat4(0, 0) == 1);
//         REQUIRE(mat4(1, 2) == 6);
        
//         std::cerr << "Matrix construction tests passed" << std::endl;
//     }
    
//     SECTION("Element access and modification") {
//         Matrix<int> mat(3, 3, 0);
        
//         // Test operator() access
//         mat(1, 1) = 99;
//         REQUIRE(mat(1, 1) == 99);
        
//         // Test at() with bounds checking
//         REQUIRE_NOTHROW(mat.at(0, 0) = 10);
//         REQUIRE_THROWS_AS(mat.at(5, 5), std::out_of_range);
        
//         // Test fill operation
//         mat.fill(7);
//         REQUIRE(mat(0, 0) == 7);
//         REQUIRE(mat(2, 2) == 7);
        
//         std::cerr << "Element access tests passed" << std::endl;
//     }
    
//     SECTION("Matrix arithmetic operations") {
//         Matrix<int> mat1{{1, 2}, {3, 4}};
//         Matrix<int> mat2{{5, 6}, {7, 8}};
        
//         // Addition
//         Matrix<int> sum = mat1 + mat2;
//         REQUIRE(sum(0, 0) == 6);  // 1 + 5
//         REQUIRE(sum(1, 1) == 12); // 4 + 8
        
//         // Subtraction
//         Matrix<int> diff = mat2 - mat1;
//         REQUIRE(diff(0, 0) == 4);  // 5 - 1
//         REQUIRE(diff(1, 1) == 4);  // 8 - 4
        
//         // Matrix multiplication
//         Matrix<int> product = mat1 * mat2;
//         REQUIRE(product(0, 0) == 19); // 1*5 + 2*7
//         REQUIRE(product(0, 1) == 22); // 1*6 + 2*8
//         REQUIRE(product(1, 0) == 43); // 3*5 + 4*7
//         REQUIRE(product(1, 1) == 50); // 3*6 + 4*8
        
//         std::cerr << "Matrix arithmetic tests passed" << std::endl;
//     }
    
//     SECTION("Advanced matrix operations") {
//         Matrix<double> mat{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
        
//         // Transpose
//         Matrix<double> transposed = mat.transpose();
//         REQUIRE(transposed.rows() == 3);
//         REQUIRE(transposed.cols() == 2);
//         REQUIRE(transposed(0, 0) == 1.0);
//         REQUIRE(transposed(2, 1) == 6.0);
        
//         // Square matrix operations
//         Matrix<double> square{{1.0, 2.0}, {3.0, 4.0}};
//         REQUIRE(square.is_square());
        
//         // Determinant (2x2 matrix: ad - bc)
//         double det = square.determinant();
//         REQUIRE(std::abs(det - (-2.0)) < 0.001); // 1*4 - 2*3 = -2
        
//         std::cerr << "Advanced matrix operations tests passed" << std::endl;
//     }
    
//     SECTION("Matrix performance benchmarks") {
//         const size_t matrix_size = 100;
        
//         BENCHMARK("Matrix multiplication performance") {
//             Matrix<double> mat1(matrix_size, matrix_size, 1.0);
//             Matrix<double> mat2(matrix_size, matrix_size, 2.0);
            
//             Matrix<double> result = mat1 * mat2;
//             return result(0, 0);
//         };
        
//         BENCHMARK("Matrix transpose performance") {
//             Matrix<double> mat(matrix_size, matrix_size, 1.0);
            
//             Matrix<double> transposed = mat.transpose();
//             return transposed(0, 0);
//         };
        
//         std::cerr << "Matrix performance benchmarks completed" << std::endl;
//     }
// }

// =============================================================================
// PERFORMANCE COMPARISON SUITE
// =============================================================================

TEST_CASE("Week 2 Performance Analysis", "[performance][day14]") {
    
    SECTION("SmartVector vs std::vector - construction") {
        const size_t test_size = 50000;
        
        BENCHMARK("SmartVector") {
            SmartVector<int> smart_vec;
            int sum = 0;
            for (size_t i = 0; i < test_size; ++i) {
                smart_vec.push_back(static_cast<int>(i));
                sum += smart_vec[7%i];
            }
            return sum;
        };

        BENCHMARK("std::vector") {
            std::vector<int> std_vec;
            int sum = 0;
            for (size_t i = 0; i < test_size; ++i) {
                std_vec.push_back(static_cast<int>(i));
                sum += std_vec[7%i];
            }
            return sum;
        };
        
        std::cerr << "Container performance comparison completed" << std::endl;
    }
}