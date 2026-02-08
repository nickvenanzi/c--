// day16_class_templates_advanced.cpp
//
// Day 16 (Advanced): Class Templates — Type Generation & Specialization
//
// Assumptions:
// - You already use std::vector and std::optional comfortably
// - You understand basic template syntax
// - You want to understand how class templates actually behave
//
// Topics:
// 1. Template class definition
// 2. Template member functions
// 3. Partial specialization
// 4. Template template parameters
//
// Compile with:
//   g++ -std=c++20 day16_class_templates_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

// ============================================================
// 1. TEMPLATE CLASS DEFINITION — TYPE GENERATORS
// ============================================================
//
// A class template is not a class.
// It is a blueprint for generating classes.
//

template<typename T>
class Box {
public:
    using value_type = T;

    Box(T value) : value_(value) {}

    T get() const { return value_; }

private:
    T value_;
};

// Instantiations are distinct, unrelated types:
//
// Box<int>      ≠ Box<double>
// Box<int>      ≠ Box<const int>
//
// Each instantiation generates its own layout and symbols.

void instantiation_examples() {
    Box<int> a(5);
    Box<double> b(3.14);

    std::cout << a.get() << '\n';
    std::cout << b.get() << '\n';
}

// ============================================================
// 2. TEMPLATE MEMBER FUNCTIONS
// ============================================================
//
// Member functions are instantiated only when used.
//

template<typename T>
class Accumulator {
public:
    void add(const T& value) {
        total_ += value;
    }

    T total() const {
        return total_;
    }

    // Templated member function inside class template
    template<typename U>
    void add_convertible(const U& value) {
        total_ += static_cast<T>(value);
    }

private:
    T total_{};
};

// Note:
// - add_convertible<U> is itself a template
// - Accumulator<int>::add_convertible<double> is distinct
//
// Two layers of template parameters exist:
//   1. Class-level T
//   2. Member-level U

// ============================================================
// 3. OUT-OF-CLASS MEMBER DEFINITIONS
// ============================================================
//
// Syntax requires full template parameter list.
//

template<typename T>
class Wrapper {
public:
    Wrapper(T v);
    T get() const;

private:
    T value_;
};

template<typename T>
Wrapper<T>::Wrapper(T v) : value_(v) {}

template<typename T>
T Wrapper<T>::get() const {
    return value_;
}

// ============================================================
// 4. PARTIAL SPECIALIZATION
// ============================================================
//
// Allowed for class templates
// NOT allowed for function templates
//

template<typename T>
class Traits {
public:
    static constexpr const char* name = "generic";
};

// Partial specialization for pointer types
template<typename T>
class Traits<T*> {
public:
    static constexpr const char* name = "pointer";
};

// Full specialization
template<>
class Traits<int> {
public:
    static constexpr const char* name = "int";
};

void specialization_examples() {
    std::cout << Traits<double>::name << '\n';   // generic
    std::cout << Traits<int>::name << '\n';      // int
    std::cout << Traits<int*>::name << '\n';     // pointer
}

// ============================================================
// 5. PARTIAL SPECIALIZATION WITH ENABLE_IF
// ============================================================
//
// SFINAE via specialization.
//

template<typename T, typename Enable = void>
class IsIntegral {
public:
    static constexpr bool value = false;
};

template<typename T>
class IsIntegral<T, std::enable_if_t<std::is_integral_v<T>>> {
public:
    static constexpr bool value = true;
};

// ============================================================
// 6. TEMPLATE TEMPLATE PARAMETERS
// ============================================================
//
// Accepting a template as a parameter.
//

template<
    typename T,
    template<typename...> class Container
>
class ContainerWrapper {
public:
    void add(const T& value) {
        container_.push_back(value);
    }

    const Container<T>& get() const {
        return container_;
    }

private:
    Container<T> container_;
};

void template_template_examples() {
    ContainerWrapper<int, std::vector> vwrap;
    vwrap.add(1);
    vwrap.add(2);

    ContainerWrapper<int, std::list> lwrap;
    lwrap.add(3);
    lwrap.add(4);

    std::cout << "vector size: " << vwrap.get().size() << '\n';
    std::cout << "list size: " << lwrap.get().size() << '\n';
}

// Nuance:
// template<typename...> required for std::vector
// because vector has multiple template parameters
// (allocator defaulted)

// ============================================================
// 7. DEPENDENT NAMES & typename REQUIREMENT
// ============================================================

template<typename T>
class DependentExample {
public:
    using ValueType = typename T::value_type;
};

// If typename is omitted:
// error: dependent name is not a type
//
// Two-phase lookup:
// - Templates parsed first
// - Names resolved during instantiation

// ============================================================
// 8. CODE BLOAT & ODR CONSIDERATIONS
// ============================================================
//
// Every unique T produces distinct instantiation.
// Can inflate binary size.
//
// Solutions:
// - Explicit instantiation
// - Reduce template surface
// - Use type erasure when appropriate
//

// Explicit instantiation example:
template class Box<int>;

// ============================================================
// MAIN
// ============================================================

int main() {

    std::cout << "\n--- Instantiation ---\n";
    instantiation_examples();

    std::cout << "\n--- Accumulator ---\n";
    Accumulator<int> acc;
    acc.add(5);
    acc.add_convertible(3.14);
    std::cout << acc.total() << '\n';

    std::cout << "\n--- Specialization ---\n";
    specialization_examples();

    std::cout << "\n--- Template Template Parameters ---\n";
    template_template_examples();

    std::cout << "\n--- IsIntegral ---\n";
    std::cout << IsIntegral<int>::value << '\n';
    std::cout << IsIntegral<double>::value << '\n';

    return 0;
}
