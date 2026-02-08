// day15_function_templates_advanced.cpp
//
// Day 15 (Advanced): Function Templates — Instantiation & Resolution Mechanics
//
// Assumptions:
// - You already write template functions comfortably
// - You want to understand how deduction and overload resolution really work
//
// Topics:
// 1. Template syntax and instantiation mechanics
// 2. Template argument deduction (rules & edge cases)
// 3. Explicit specialization
// 4. Template overloading resolution
//
// Compile with:
//   g++ -std=c++20 day15_function_templates_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <type_traits>
#include <string>

// ============================================================
// 1. TEMPLATE SYNTAX & INSTANTIATION MECHANICS
// ============================================================
//
// Templates are blueprints.
// Code is generated only when instantiated.
//
// Instantiation happens when:
// - A function is called
// - Its address is taken
// - It is otherwise ODR-used
//

template<typename T>
T identity(T value) {
    return value;
}

// No code generated yet.
//
// Instantiated when used:
void instantiation_examples() {
    auto a = identity(42);          // T = int
    auto b = identity(3.14);        // T = double
    auto c = identity(std::string("hi")); // T = std::string

    (void)a; (void)b; (void)c;
}

// You can explicitly force instantiation:
template int identity<int>(int);

// ============================================================
// 2. TEMPLATE ARGUMENT DEDUCTION — RULES & EDGE CASES
// ============================================================

// Basic deduction
template<typename T>
void print_type(T) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

// Deduction ignores:
// - top-level const
// - references
// - value category (unless forwarding references)

void deduction_basics() {
    int x = 5;
    const int cx = 10;

    print_type(x);   // T = int
    print_type(cx);  // T = int (top-level const dropped)
}

// ----------------------------------------------
// Reference collapsing & forwarding references
// ----------------------------------------------

template<typename T>
void forwarding(T&& param) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

// Rules:
// - If argument is lvalue → T deduced as T&
// - If rvalue → T deduced as T
//
// Then reference collapsing applies.

void forwarding_examples() {
    int x = 5;

    forwarding(x);   // T = int&, param = int&
    forwarding(42);  // T = int, param = int&&
}

// ----------------------------------------------
// Deduction fails in non-deduced contexts
// ----------------------------------------------

template<typename T>
void non_deduced(typename std::remove_reference<T>::type) {
    std::cout << "Non-deduced context\n";
}

// Must specify explicitly:
// non_deduced<int>(5);

// ============================================================
// 3. EXPLICIT SPECIALIZATION
// ============================================================

template<typename T>
void describe(T) {
    std::cout << "Generic template\n";
}

// Explicit specialization
template<>
void describe<int>(int) {
    std::cout << "Specialization for int\n";
}

// Important:
// - Specializations must appear after primary template
// - Specialization chosen after overload resolution picks template
//
// NOT partial specialization (not allowed for function templates)

// Workaround for partial specialization: use overloading

template<typename T>
void describe_ptr(T*) {
    std::cout << "Pointer type\n";
}

// ============================================================
// 4. TEMPLATE OVERLOADING RESOLUTION
// ============================================================
//
// Ordering rules:
// 1. Non-template functions preferred over templates
// 2. More specialized templates preferred
// 3. Partial ordering between templates determines winner
//

// Non-template
void process(int) {
    std::cout << "Non-template int\n";
}

// Generic template
template<typename T>
void process(T) {
    std::cout << "Generic template\n";
}

// More specialized template
template<typename T>
void process(T*) {
    std::cout << "Pointer template\n";
}

void overload_resolution_examples() {
    int x = 0;
    int* px = &x;

    process(x);   // Non-template wins
    process(px);  // Pointer template wins
    process(3.14); // Generic template
}

// ----------------------------------------------
// Partial ordering between templates
// ----------------------------------------------

template<typename T>
void compare(T, T) {
    std::cout << "Same types\n";
}

template<typename T, typename U>
void compare(T, U) {
    std::cout << "Different types\n";
}

void partial_ordering_examples() {
    compare(1, 2);        // Same types
    compare(1, 2.0);      // Different types
}

// Compiler determines which template is more specialized.

// ============================================================
// 5. SFINAE & SUBSTITUTION FAILURE (Preview)
// ============================================================

template<typename T>
auto only_integral(T value) -> std::enable_if_t<std::is_integral_v<T>, T>
{
    return value;
}

// If T is not integral, substitution fails —
// overload is removed from candidate set.

void sfinae_example() {
    auto x = only_integral(42);
    (void)x;

    // only_integral(3.14); // error: no matching function
}

// ============================================================
// 6. TEMPLATE INSTANTIATION COST MODEL
// ============================================================
//
// - Each distinct T generates separate code
// - Can increase binary size
// - Enables inlining & optimization
// - ODR violations possible across translation units
//
// Explicit instantiation can control code bloat.
//

// ============================================================
// MAIN
// ============================================================

int main() {

    std::cout << "\n--- Instantiation ---\n";
    instantiation_examples();

    std::cout << "\n--- Deduction ---\n";
    deduction_basics();

    std::cout << "\n--- Forwarding ---\n";
    forwarding_examples();

    std::cout << "\n--- Specialization ---\n";
    describe(3.14);
    describe(42);

    std::cout << "\n--- Overload Resolution ---\n";
    overload_resolution_examples();

    std::cout << "\n--- Partial Ordering ---\n";
    partial_ordering_examples();

    std::cout << "\n--- SFINAE ---\n";
    sfinae_example();

    return 0;
    
}
