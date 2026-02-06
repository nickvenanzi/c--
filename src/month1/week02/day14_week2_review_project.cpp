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
// PROJECT 2: Shape Hierarchy - Inheritance, polymorphism, and virtual functions
// PROJECT 3: Exception-Safe Resource Manager - RAII with exception handling
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
    SmartVector();
    explicit SmartVector(size_t initial_capacity);
    SmartVector(size_t count, const T& value);
    SmartVector(std::initializer_list<T> init);
    SmartVector(const SmartVector& other);
    SmartVector& operator=(const SmartVector& other);
    ~SmartVector();
    
    // Element access
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    T& at(size_t index);
    const T& at(size_t index) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data();
    const T* data() const;
    
    // Capacity
    bool empty() const;
    size_t size() const;
    size_t capacity() const;
    void reserve(size_t new_capacity);
    void shrink_to_fit();
    
    // Modifiers
    void clear();
    void push_back(const T& value);
    void pop_back();
    void resize(size_t new_size);
    void resize(size_t new_size, const T& value);
    
    // Operators
    SmartVector operator+(const SmartVector& other) const;
    SmartVector& operator+=(const SmartVector& other);
    bool operator==(const SmartVector& other) const;
    bool operator!=(const SmartVector& other) const;
    
    // Iterator support (basic)
    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;
    
    // Debug/testing methods
    size_t get_allocation_count() const;
    size_t get_reallocation_count() const;

private:
    T* data_;
    size_t size_;
    size_t capacity_;
    static size_t allocation_count_;
    static size_t reallocation_count_;
    
    void reallocate(size_t new_capacity);
    void destroy_elements();
};

// =============================================================================
// PROJECT 2 SPECIFICATION: Shape Hierarchy
// =============================================================================

class Shape {
public:
    Shape(const std::string& name);
    virtual ~Shape() = default;
    
    // Pure virtual functions
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void draw() const = 0;
    virtual std::unique_ptr<Shape> clone() const = 0;
    
    // Virtual functions with default implementation
    virtual void move(double dx, double dy);
    virtual void scale(double factor);
    virtual std::string get_info() const;
    
    // Non-virtual functions
    const std::string& get_name() const;
    void set_name(const std::string& name);
    
protected:
    std::string name_;
    double x_, y_;  // Position
};

class Circle : public Shape {
public:
    Circle(double radius, double x = 0, double y = 0);
    
    // Override pure virtual functions
    double area() const override;
    double perimeter() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;
    
    // Override virtual functions
    void scale(double factor) override;
    std::string get_info() const override;
    
    // Circle-specific methods
    double get_radius() const;
    void set_radius(double radius);

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height, double x = 0, double y = 0);
    
    // Override pure virtual functions
    double area() const override;
    double perimeter() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;
    
    // Override virtual functions
    void scale(double factor) override;
    std::string get_info() const override;
    
    // Rectangle-specific methods
    double get_width() const;
    double get_height() const;
    void set_dimensions(double width, double height);

private:
    double width_, height_;
};

class Triangle : public Shape {
public:
    Triangle(double side_a, double side_b, double side_c, double x = 0, double y = 0);
    
    // Override pure virtual functions
    double area() const override;
    double perimeter() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;
    
    // Override virtual functions
    void scale(double factor) override;
    std::string get_info() const override;
    
    // Triangle-specific methods
    bool is_valid() const;
    std::string get_triangle_type() const;

private:
    double side_a_, side_b_, side_c_;
    bool validate_triangle() const;
};

// =============================================================================
// PROJECT 3 SPECIFICATION: Exception-Safe Resource Manager
// =============================================================================

class ResourceException : public std::runtime_error {
public:
    explicit ResourceException(const std::string& message);
};

class FileResource {
public:
    explicit FileResource(const std::string& filename);
    ~FileResource();
    
    // Delete copy operations for RAII safety
    FileResource(const FileResource&) = delete;
    FileResource& operator=(const FileResource&) = delete;
    
    // Move operations
    FileResource(FileResource&& other) noexcept;
    FileResource& operator=(FileResource&& other) noexcept;
    
    // File operations
    void write(const std::string& data);
    std::string read();
    void flush();
    bool is_open() const;
    const std::string& get_filename() const;
    
    // Exception-safe operations
    void safe_write(const std::string& data);
    std::string safe_read();

private:
    std::string filename_;
    void* file_handle_;  // Simulated file handle
    bool is_open_;
    
    void close();
};

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();
    
    // Resource management
    void add_resource(const std::string& name, std::unique_ptr<FileResource> resource);
    FileResource* get_resource(const std::string& name);
    void remove_resource(const std::string& name);
    void clear_all();
    
    // Exception-safe batch operations
    void batch_write(const std::vector<std::pair<std::string, std::string>>& operations);
    std::vector<std::string> batch_read(const std::vector<std::string>& resource_names);
    
    // Statistics
    size_t get_resource_count() const;
    std::vector<std::string> get_resource_names() const;

private:
    std::vector<std::pair<std::string, std::unique_ptr<FileResource>>> resources_;
    
    void cleanup_resources() noexcept;
};

// =============================================================================
// PROJECT 4 SPECIFICATION: Template Matrix Class
// =============================================================================

template<typename T>
class Matrix {
public:
    // Constructors and destructor
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, const T& initial_value);
    Matrix(std::initializer_list<std::initializer_list<T>> init);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();
    
    // Element access
    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;
    T& at(size_t row, size_t col);
    const T& at(size_t row, size_t col) const;
    
    // Capacity
    size_t rows() const;
    size_t cols() const;
    size_t size() const;
    bool empty() const;
    
    // Modifiers
    void resize(size_t new_rows, size_t new_cols);
    void resize(size_t new_rows, size_t new_cols, const T& value);
    void fill(const T& value);
    void clear();
    
    // Matrix operations
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const T& scalar);
    
    // Comparison operators
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    
    // Matrix-specific operations
    Matrix transpose() const;
    T determinant() const;  // For square matrices
    Matrix inverse() const; // For square matrices
    bool is_square() const;
    
    // Utility functions
    void print() const;
    std::string to_string() const;

private:
    T** data_;
    size_t rows_;
    size_t cols_;
    
    void allocate_memory();
    void deallocate_memory();
    void copy_from(const Matrix& other);
};

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
        size_t initial_allocations = vec.get_allocation_count();
        
        // Test push_back and automatic growth
        for (int i = 0; i < 100; ++i) {
            vec.push_back(i);
        }
        
        REQUIRE(vec.size() == 100);
        REQUIRE(vec.capacity() >= 100);
        REQUIRE(vec.get_allocation_count() > initial_allocations);
        
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

// =============================================================================
// TEST SUITE FOR Shape Hierarchy
// =============================================================================

TEST_CASE("Shape Hierarchy Implementation Tests", "[shapes][project2][day14]") {
    
    SECTION("Basic shape creation and polymorphism") {
        std::cerr << "\n=== Shape Hierarchy Tests ===" << std::endl;
        
        // Create shapes through base class pointers
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Circle>(5.0, 0, 0));
        shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0, 1, 1));
        shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0, 2, 2));
        
        // Test polymorphic behavior
        for (const auto& shape : shapes) {
            std::cerr << shape->get_info() << std::endl;
            shape->draw();
            
            REQUIRE(shape->area() > 0);
            REQUIRE(shape->perimeter() > 0);
        }
        
        std::cerr << "Polymorphism tests passed" << std::endl;
    }
    
    SECTION("Circle-specific functionality") {
        Circle circle(3.0, 1, 2);
        
        // Test area and perimeter calculations
        double expected_area = 3.14159 * 3.0 * 3.0;
        double expected_perimeter = 2 * 3.14159 * 3.0;
        
        REQUIRE(std::abs(circle.area() - expected_area) < 0.01);
        REQUIRE(std::abs(circle.perimeter() - expected_perimeter) < 0.01);
        
        // Test scaling
        circle.scale(2.0);
        REQUIRE(std::abs(circle.get_radius() - 6.0) < 0.01);
        
        std::cerr << "Circle functionality tests passed" << std::endl;
    }
    
    SECTION("Rectangle-specific functionality") {
        Rectangle rect(4.0, 5.0, 0, 0);
        
        // Test area and perimeter
        REQUIRE(rect.area() == 20.0);
        REQUIRE(rect.perimeter() == 18.0);
        
        // Test dimension modification
        rect.set_dimensions(6.0, 8.0);
        REQUIRE(rect.get_width() == 6.0);
        REQUIRE(rect.get_height() == 8.0);
        REQUIRE(rect.area() == 48.0);
        
        std::cerr << "Rectangle functionality tests passed" << std::endl;
    }
    
    SECTION("Triangle validation and types") {
        // Valid triangle
        Triangle valid_triangle(3.0, 4.0, 5.0);
        REQUIRE(valid_triangle.is_valid());
        REQUIRE(valid_triangle.area() > 0);
        
        // Invalid triangle (violates triangle inequality)
        Triangle invalid_triangle(1.0, 2.0, 5.0);
        REQUIRE_FALSE(invalid_triangle.is_valid());
        
        // Test triangle type classification
        Triangle equilateral(5.0, 5.0, 5.0);
        REQUIRE(equilateral.get_triangle_type() == "Equilateral");
        
        std::cerr << "Triangle validation tests passed" << std::endl;
    }
    
    SECTION("Shape cloning and virtual copy") {
        Circle original(7.0, 3, 4);
        auto cloned = original.clone();
        
        REQUIRE(cloned->area() == original.area());
        REQUIRE(cloned->perimeter() == original.perimeter());
        REQUIRE(cloned->get_name() == original.get_name());
        
        std::cerr << "Shape cloning tests passed" << std::endl;
    }
    
    SECTION("Performance comparison") {
        const size_t num_shapes = 10000;
        
        BENCHMARK("Virtual function calls") {
            std::vector<std::unique_ptr<Shape>> shapes;
            for (size_t i = 0; i < num_shapes; ++i) {
                shapes.push_back(std::make_unique<Circle>(1.0));
            }
            
            volatile double total_area = 0;
            for (const auto& shape : shapes) {
                total_area += shape->area();  // Virtual function call
            }
            return total_area;
        };
        
        BENCHMARK("Direct function calls") {
            std::vector<Circle> circles;
            for (size_t i = 0; i < num_shapes; ++i) {
                circles.emplace_back(1.0);
            }
            
            volatile double total_area = 0;
            for (const auto& circle : circles) {
                total_area += circle.area();  // Direct function call
            }
            return total_area;
        };
        
        std::cerr << "Performance benchmarks completed" << std::endl;
    }
}

// =============================================================================
// TEST SUITE FOR Exception-Safe Resource Manager
// =============================================================================

TEST_CASE("Resource Manager Implementation Tests", "[resources][project3][day14]") {
    
    SECTION("Basic resource management") {
        std::cerr << "\n=== Resource Manager Tests ===" << std::endl;
        
        ResourceManager manager;
        
        // Add resources
        manager.add_resource("file1", std::make_unique<FileResource>("test1.txt"));
        manager.add_resource("file2", std::make_unique<FileResource>("test2.txt"));
        
        REQUIRE(manager.get_resource_count() == 2);
        
        // Access resources
        FileResource* file1 = manager.get_resource("file1");
        REQUIRE(file1 != nullptr);
        REQUIRE(file1->get_filename() == "test1.txt");
        
        // Remove resource
        manager.remove_resource("file1");
        REQUIRE(manager.get_resource_count() == 1);
        REQUIRE(manager.get_resource("file1") == nullptr);
        
        std::cerr << "Basic resource management tests passed" << std::endl;
    }
    
    SECTION("Exception safety in resource operations") {
        ResourceManager manager;
        manager.add_resource("test", std::make_unique<FileResource>("test.txt"));
        
        FileResource* resource = manager.get_resource("test");
        REQUIRE(resource != nullptr);
        
        // Test exception-safe write
        REQUIRE_NOTHROW(resource->safe_write("Hello, World!"));
        
        // Test exception-safe read
        std::string content;
        REQUIRE_NOTHROW(content = resource->safe_read());
        REQUIRE(content == "Hello, World!");
        
        std::cerr << "Exception safety tests passed" << std::endl;
    }
    
    SECTION("Batch operations with exception handling") {
        ResourceManager manager;
        manager.add_resource("batch1", std::make_unique<FileResource>("batch1.txt"));
        manager.add_resource("batch2", std::make_unique<FileResource>("batch2.txt"));
        
        // Test batch write
        std::vector<std::pair<std::string, std::string>> write_ops = {
            {"batch1", "Data for file 1"},
            {"batch2", "Data for file 2"}
        };
        
        REQUIRE_NOTHROW(manager.batch_write(write_ops));
        
        // Test batch read
        std::vector<std::string> read_names = {"batch1", "batch2"};
        std::vector<std::string> results;
        
        REQUIRE_NOTHROW(results = manager.batch_read(read_names));
        REQUIRE(results.size() == 2);
        REQUIRE(results[0] == "Data for file 1");
        REQUIRE(results[1] == "Data for file 2");
        
        std::cerr << "Batch operations tests passed" << std::endl;
    }
    
    SECTION("Move semantics for FileResource") {
        FileResource original("movable.txt");
        original.write("Original data");
        
        // Test move constructor
        FileResource moved(std::move(original));
        REQUIRE(moved.is_open());
        REQUIRE(moved.get_filename() == "movable.txt");
        REQUIRE_FALSE(original.is_open());  // Original should be in moved-from state
        
        std::cerr << "Move semantics tests passed" << std::endl;
    }
    
    SECTION("Exception handling stress test") {
        const size_t num_resources = 1000;
        
        BENCHMARK("Resource creation and cleanup") {
            ResourceManager manager;
            
            for (size_t i = 0; i < num_resources; ++i) {
                std::string name = "resource" + std::to_string(i);
                std::string filename = name + ".txt";
                manager.add_resource(name, std::make_unique<FileResource>(filename));
            }
            
            return manager.get_resource_count();
        };
        
        std::cerr << "Exception handling stress test completed" << std::endl;
    }
}

// =============================================================================
// TEST SUITE FOR Template Matrix
// =============================================================================

TEST_CASE("Matrix Implementation Tests", "[matrix][project4][day14]") {
    
    SECTION("Matrix construction and basic operations") {
        std::cerr << "\n=== Matrix Tests ===" << std::endl;
        
        // Default constructor
        Matrix<int> mat1;
        REQUIRE(mat1.empty());
        REQUIRE(mat1.rows() == 0);
        REQUIRE(mat1.cols() == 0);
        
        // Dimension constructor
        Matrix<double> mat2(3, 4);
        REQUIRE(mat2.rows() == 3);
        REQUIRE(mat2.cols() == 4);
        REQUIRE_FALSE(mat2.empty());
        
        // Constructor with initial value
        Matrix<int> mat3(2, 2, 42);
        REQUIRE(mat3(0, 0) == 42);
        REQUIRE(mat3(1, 1) == 42);
        
        // Initializer list constructor
        Matrix<int> mat4{{1, 2, 3}, {4, 5, 6}};
        REQUIRE(mat4.rows() == 2);
        REQUIRE(mat4.cols() == 3);
        REQUIRE(mat4(0, 0) == 1);
        REQUIRE(mat4(1, 2) == 6);
        
        std::cerr << "Matrix construction tests passed" << std::endl;
    }
    
    SECTION("Element access and modification") {
        Matrix<int> mat(3, 3, 0);
        
        // Test operator() access
        mat(1, 1) = 99;
        REQUIRE(mat(1, 1) == 99);
        
        // Test at() with bounds checking
        REQUIRE_NOTHROW(mat.at(0, 0) = 10);
        REQUIRE_THROWS_AS(mat.at(5, 5), std::out_of_range);
        
        // Test fill operation
        mat.fill(7);
        REQUIRE(mat(0, 0) == 7);
        REQUIRE(mat(2, 2) == 7);
        
        std::cerr << "Element access tests passed" << std::endl;
    }
    
    SECTION("Matrix arithmetic operations") {
        Matrix<int> mat1{{1, 2}, {3, 4}};
        Matrix<int> mat2{{5, 6}, {7, 8}};
        
        // Addition
        Matrix<int> sum = mat1 + mat2;
        REQUIRE(sum(0, 0) == 6);  // 1 + 5
        REQUIRE(sum(1, 1) == 12); // 4 + 8
        
        // Subtraction
        Matrix<int> diff = mat2 - mat1;
        REQUIRE(diff(0, 0) == 4);  // 5 - 1
        REQUIRE(diff(1, 1) == 4);  // 8 - 4
        
        // Matrix multiplication
        Matrix<int> product = mat1 * mat2;
        REQUIRE(product(0, 0) == 19); // 1*5 + 2*7
        REQUIRE(product(0, 1) == 22); // 1*6 + 2*8
        REQUIRE(product(1, 0) == 43); // 3*5 + 4*7
        REQUIRE(product(1, 1) == 50); // 3*6 + 4*8
        
        std::cerr << "Matrix arithmetic tests passed" << std::endl;
    }
    
    SECTION("Advanced matrix operations") {
        Matrix<double> mat{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
        
        // Transpose
        Matrix<double> transposed = mat.transpose();
        REQUIRE(transposed.rows() == 3);
        REQUIRE(transposed.cols() == 2);
        REQUIRE(transposed(0, 0) == 1.0);
        REQUIRE(transposed(2, 1) == 6.0);
        
        // Square matrix operations
        Matrix<double> square{{1.0, 2.0}, {3.0, 4.0}};
        REQUIRE(square.is_square());
        
        // Determinant (2x2 matrix: ad - bc)
        double det = square.determinant();
        REQUIRE(std::abs(det - (-2.0)) < 0.001); // 1*4 - 2*3 = -2
        
        std::cerr << "Advanced matrix operations tests passed" << std::endl;
    }
    
    SECTION("Matrix performance benchmarks") {
        const size_t matrix_size = 100;
        
        BENCHMARK("Matrix multiplication performance") {
            Matrix<double> mat1(matrix_size, matrix_size, 1.0);
            Matrix<double> mat2(matrix_size, matrix_size, 2.0);
            
            Matrix<double> result = mat1 * mat2;
            return result(0, 0);
        };
        
        BENCHMARK("Matrix transpose performance") {
            Matrix<double> mat(matrix_size, matrix_size, 1.0);
            
            Matrix<double> transposed = mat.transpose();
            return transposed(0, 0);
        };
        
        std::cerr << "Matrix performance benchmarks completed" << std::endl;
    }
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

TEST_CASE("Week 2 Integration Tests", "[integration][day14]") {
    
    SECTION("Combined usage of all projects") {
        std::cerr << "\n=== Integration Tests ===" << std::endl;
        
        // Use SmartVector to store shapes
        SmartVector<std::unique_ptr<Shape>> shape_container;
        shape_container.push_back(std::make_unique<Circle>(5.0));
        shape_container.push_back(std::make_unique<Rectangle>(3.0, 4.0));
        
        // Calculate total area using polymorphism
        double total_area = 0.0;
        for (size_t i = 0; i < shape_container.size(); ++i) {
            total_area += shape_container[i]->area();
        }
        
        REQUIRE(total_area > 0);
        
        // Use Matrix to store shape properties
        Matrix<double> shape_properties(static_cast<int>(shape_container.size()), 3);
        for (size_t i = 0; i < shape_container.size(); ++i) {
            shape_properties(i, 0) = shape_container[i]->area();
            shape_properties(i, 1) = shape_container[i]->perimeter();
            shape_properties(i, 2) = static_cast<double>(i); // ID
        }
        
        REQUIRE(shape_properties.rows() == 2);
        REQUIRE(shape_properties.cols() == 3);
        
        // Use ResourceManager for logging
        ResourceManager logger;
        logger.add_resource("log", std::make_unique<FileResource>("integration_log.txt"));
        
        FileResource* log_file = logger.get_resource("log");
        REQUIRE(log_file != nullptr);
        
        log_file->safe_write("Integration test completed successfully");
        std::string log_content = log_file->safe_read();
        REQUIRE(log_content == "Integration test completed successfully");
        
        std::cerr << "Integration tests passed" << std::endl;
    }
    
    SECTION("Exception safety across all components") {
        // Test that all components handle exceptions gracefully
        REQUIRE_NOTHROW({
            SmartVector<int> vec{1, 2, 3};
            Matrix<double> mat(2, 2, 1.0);
            ResourceManager manager;
            
            // Operations that might throw
            vec.at(0);  // Should not throw
            mat.at(0, 0);  // Should not throw
            manager.add_resource("test", std::make_unique<FileResource>("test.txt"));
        });
        
        std::cerr << "Exception safety integration tests passed" << std::endl;
    }
}

// =============================================================================
// PERFORMANCE COMPARISON SUITE
// =============================================================================

TEST_CASE("Week 2 Performance Analysis", "[performance][day14]") {
    
    SECTION("Container performance comparison") {
        const size_t test_size = 50000;
        
        BENCHMARK("SmartVector vs std::vector - construction") {
            SmartVector<int> smart_vec;
            std::vector<int> std_vec;
            
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < test_size; ++i) {
                smart_vec.push_back(static_cast<int>(i));
            }
            auto mid = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < test_size; ++i) {
                std_vec.push_back(static_cast<int>(i));
            }
            auto end = std::chrono::high_resolution_clock::now();
            
            auto smart_time = std::chrono::duration_cast<std::chrono::microseconds>(mid - start);
            auto std_time = std::chrono::duration_cast<std::chrono::microseconds>(end - mid);
            
            std::cerr << "SmartVector time: " << smart_time.count() << " μs" << std::endl;
            std::cerr << "std::vector time: " << std_time.count() << " μs" << std::endl;
            
            return smart_vec.size() + std_vec.size();
        };
        
        std::cerr << "Container performance comparison completed" << std::endl;
    }
    
    SECTION("Polymorphism overhead analysis") {
        const size_t num_operations = 100000;
        
        // Direct calls
        Circle direct_circle(1.0);
        auto start_direct = std::chrono::high_resolution_clock::now();
        volatile double direct_total = 0;
        for (size_t i = 0; i < num_operations; ++i) {
            direct_total += direct_circle.area();
        }
        auto end_direct = std::chrono::high_resolution_clock::now();
        
        // Virtual calls
        std::unique_ptr<Shape> virtual_circle = std::make_unique<Circle>(1.0);
        auto start_virtual = std::chrono::high_resolution_clock::now();
        volatile double virtual_total = 0;
        for (size_t i = 0; i < num_operations; ++i) {
            virtual_total += virtual_circle->area();
        }
        auto end_virtual = std::chrono::high_resolution_clock::now();
        
        auto direct_time = std::chrono::duration_cast<std::chrono::microseconds>(end_direct - start_direct);
        auto virtual_time = std::chrono::duration_cast<std::chrono::microseconds>(end_virtual - start_virtual);
        
        std::cerr << "Direct calls time: " << direct_time.count() << " μs" << std::endl;
        std::cerr << "Virtual calls time: " << virtual_time.count() << " μs" << std::endl;
        std::cerr << "Virtual overhead: " << (virtual_time.count() - direct_time.count()) << " μs" << std::endl;
        
        REQUIRE(direct_total > 0);
        REQUIRE(virtual_total > 0);
        
        std::cerr << "Polymorphism overhead analysis completed" << std::endl;
    }
}

// =============================================================================
// FINAL PROJECT SUMMARY
// =============================================================================

TEST_CASE("Week 2 Project Summary", "[summary][day14]") {
    
    SECTION("Implementation completeness check") {
        std::cerr << "\n=== Week 2 Project Summary ===" << std::endl;
        
        // Check that all major components can be instantiated
        bool smartvector_implemented = false;
        bool shapes_implemented = false;
        bool resources_implemented = false;
        bool matrix_implemented = false;
        
        try {
            SmartVector<int> vec{1, 2, 3};
            smartvector_implemented = (vec.size() == 3);
        } catch (...) {
            smartvector_implemented = false;
        }
        
        try {
            auto circle = std::make_unique<Circle>(1.0);
            shapes_implemented = (circle->area() > 0);
        } catch (...) {
            shapes_implemented = false;
        }
        
        try {
            ResourceManager manager;
            resources_implemented = (manager.get_resource_count() == 0);
        } catch (...) {
            resources_implemented = false;
        }
        
        try {
            Matrix<int> mat(2, 2, 1);
            matrix_implemented = (mat.rows() == 2 && mat.cols() == 2);
        } catch (...) {
            matrix_implemented = false;
        }
        
        std::cerr << "SmartVector implemented: " << (smartvector_implemented ? "YES" : "NO") << std::endl;
        std::cerr << "Shape hierarchy implemented: " << (shapes_implemented ? "YES" : "NO") << std::endl;
        std::cerr << "Resource manager implemented: " << (resources_implemented ? "YES" : "NO") << std::endl;
        std::cerr << "Matrix template implemented: " << (matrix_implemented ? "YES" : "NO") << std::endl;
        
        // All should be implemented for full credit
        REQUIRE(smartvector_implemented);
        REQUIRE(shapes_implemented);
        REQUIRE(resources_implemented);
        REQUIRE(matrix_implemented);
        
        std::cerr << "\n🎉 Week 2 OOP Projects Complete! 🎉" << std::endl;
        std::cerr << "You have successfully implemented:" << std::endl;
        std::cerr << "✓ Custom container with RAII and operator overloading" << std::endl;
        std::cerr << "✓ Polymorphic shape hierarchy with virtual functions" << std::endl;
        std::cerr << "✓ Exception-safe resource management system" << std::endl;
        std::cerr << "✓ Generic template matrix with advanced operations" << std::endl;
        
        cpp_mastery::Logger::info("Week 2 OOP mastery achieved!");
    }
}