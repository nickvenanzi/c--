// day17_template_metaprogramming_basics_advanced.cpp
//
// Day 17 (Advanced): Template Metaprogramming Basics
//
// Assumptions:
// - You understand templates and specialization
// - You want to understand compile-time computation mechanics
//
// Topics:
// 1. Compile-time computation
// 2. Type traits and SFINAE basics
// 3. Template recursion
// 4. Conditional compilation (type-based)
//
// Compile with:
//   g++ -std=c++20 day17_template_metaprogramming_basics_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <type_traits>
#include <string>

// ============================================================
// 1. COMPILE-TIME COMPUTATION
// ============================================================
//
// Templates allow computation during compilation.
// Historically done via template recursion.
// Now often replaced by constexpr — but mechanics matter.
//

// Classic template metaprogramming factorial
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

// Base case specialization
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

static_assert(Factorial<5>::value == 120);

// This is purely compile-time.
// No runtime code generated.

// ============================================================
// 2. TYPE-LEVEL COMPUTATION
// ============================================================
//
// Metaprogramming isn't just values — it's type logic.
//

template<typename T>
struct AddPointer {
    using type = T*;
};

static_assert(std::is_same_v<AddPointer<int>::type, int*>);

// Alias template (modern style)
template<typename T>
using AddPointer_t = T*;

static_assert(std::is_same_v<AddPointer_t<double>, double*>);

// ============================================================
// 3. TEMPLATE RECURSION (TYPE LIST PROCESSING)
// ============================================================

template<typename... Ts>
struct TypeCount;

template<>
struct TypeCount<> {
    static constexpr std::size_t value = 0;
};

template<typename T, typename... Ts>
struct TypeCount<T, Ts...> {
    static constexpr std::size_t value = 1 + TypeCount<Ts...>::value;
};

static_assert(TypeCount<int, double, char>::value == 3);

// Pattern:
// - Base case
// - Recursive case
//
// This mirrors runtime recursion, but unfolds at compile time.

// ============================================================
// 4. CONDITIONAL TYPE SELECTION
// ============================================================
//
// Classic type-level if
//

template<bool Condition, typename TrueType, typename FalseType>
struct Conditional {
    using type = TrueType;
};

template<typename TrueType, typename FalseType>
struct Conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

static_assert(std::is_same_v<
    Conditional<true, int, double>::type,
    int
>);

// Modern equivalent:
static_assert(std::is_same_v<
    std::conditional_t<false, int, double>,
    double
>);

// ============================================================
// 5. TYPE TRAITS & SFINAE BASICS
// ============================================================
//
// SFINAE = Substitution Failure Is Not An Error
//
// If template substitution fails in certain contexts,
// that overload is removed from the candidate set.
//

// Enable only for integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
add_one(T value) {
    return value + 1;
}

// If T is not integral → substitution fails
// function removed from overload resolution

void sfinae_examples() {
    auto x = add_one(5);  // OK
    (void)x;

    // add_one(3.14); // error: no matching function
}

// ============================================================
// 6. DETECTION IDIOM (BASIC FORM)
// ============================================================
//
// Detect if a type has a nested ::value_type
//

template<typename, typename = void>
struct HasValueType : std::false_type {};

template<typename T>
struct HasValueType<T, std::void_t<typename T::value_type>>
    : std::true_type {};

static_assert(HasValueType<std::string>::value == true);
static_assert(HasValueType<int>::value == false);

// Mechanism:
// - If T::value_type is ill-formed,
//   substitution fails in specialization
// - Falls back to primary template

// ============================================================
// 7. IF CONSTEXPR — CONDITIONAL COMPILATION (MODERN)
// ============================================================

template<typename T>
void print_category(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral: " << value << '\n';
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating: " << value << '\n';
    }
    else {
        std::cout << "Other\n";
    }
}

// Unlike runtime if:
// - Non-selected branches are not instantiated
// - No SFINAE required
// - Cleaner and safer

// ============================================================
// 8. CONDITIONAL INHERITANCE
// ============================================================

template<bool Enable>
struct LoggerBase {};

template<>
struct LoggerBase<true> {
    void log(const std::string& msg) const {
        std::cout << "[LOG] " << msg << '\n';
    }
};

template<bool EnableLogging>
class Service : public LoggerBase<EnableLogging> {
public:
    void run() {
        if constexpr (EnableLogging) {
            this->log("Service running");
        }
    }
};

// Zero runtime cost when disabled.

// ============================================================
// 9. COST MODEL OF TEMPLATE METAPROGRAMMING
// ============================================================
//
// Compile-time cost:
// - Increased compile times
// - Large template instantiations
//
// Runtime cost:
// - Usually zero
//
// Binary cost:
// - Potential code bloat
//
// Trade-off:
// - Move complexity to compile-time
// - Gain runtime performance & type safety
//

// ============================================================
// MAIN
// ============================================================

int main() {

    std::cout << "\n--- Compile-time factorial ---\n";
    std::cout << Factorial<6>::value << '\n';

    std::cout << "\n--- TypeCount ---\n";
    std::cout << TypeCount<int, double, char, bool>::value << '\n';

    std::cout << "\n--- SFINAE ---\n";
    sfinae_examples();

    std::cout << "\n--- if constexpr ---\n";
    print_category(42);
    print_category(3.14);
    print_category("hello");

    std::cout << "\n--- Conditional inheritance ---\n";
    Service<true> s1;
    s1.run();

    Service<false> s2;
    s2.run();

    return 0;
}
