// day10_operator_overloading_advanced.cpp
//
// Day 10 (Advanced): Operator Overloading — Semantics, Contracts, Pitfalls
//
// Assumptions:
// - You already know operator overloading syntax
// - You care about correctness, discoverability, ABI, and performance
//
// Topics:
// 1. Arithmetic and comparison operators
// 2. Assignment and compound assignment
// 3. Stream insertion/extraction
// 4. Operator overloading best practices (when NOT to overload)
//
// Compile with:
//   g++ -std=c++20 day10_operator_overloading_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <compare>
#include <string>
#include <cassert>

// ============================================================
// 1. ARITHMETIC OPERATORS — VALUE SEMANTICS VS MUTATION
// ============================================================

struct Vec2 {
    float x{0}, y{0};

    // Compound assignment SHOULD be a member
    // Mutates *this*
    Vec2& operator+=(const Vec2& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs) noexcept {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
};

// Free function arithmetic operators
// Implemented in terms of compound assignment
inline Vec2 operator+(Vec2 lhs, const Vec2& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

inline Vec2 operator-(Vec2 lhs, const Vec2& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

// Key nuances:
// - Take lhs BY VALUE to enable move elision
// - Enables expressions like a + b + c
// - Mirrors std::string and std::chrono design

// ============================================================
// 2. COMPARISON OPERATORS — TOTAL ORDERING AND DEFAULTING
// ============================================================

struct Version {
    int major{};
    int minor{};
    int patch{};

    // C++20 spaceship operator
    auto operator<=>(const Version&) const = default;
};

// What this gives you:
// - ==
// - !=
// - <
// - <=
// - >
// - >=
//
// And the ordering is lexicographical by member order

static_assert(Version{1,2,3} < Version{2,0,0});
static_assert(Version{1,2,3} == Version{1,2,3});

// Nuance:
// - Member declaration order defines comparison order
// - Floating point? Beware NaNs → partial ordering
// - Custom invariants may require manual implementation

// ============================================================
// 3. ASSIGNMENT AND COMPOUND ASSIGNMENT — STRONG GUARANTEES
// ============================================================

struct Buffer {
    int* data{nullptr};
    std::size_t size{0};

    Buffer(std::size_t n)
        : data(new int[n]{}), size(n)
    {}

    ~Buffer() {
        delete[] data;
    }

    // COPY CONSTRUCTOR
    Buffer(const Buffer& other)
        : data(new int[other.size]), size(other.size)
    {
        std::copy(other.data, other.data + size, data);
    }

    // COPY ASSIGNMENT — STRONG EXCEPTION SAFETY
    Buffer& operator=(Buffer rhs) {
        // Copy-and-swap idiom
        swap(rhs);
        return *this;
    }

    void swap(Buffer& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
    }
};

// Nuances:
// - Assignment must handle self-assignment
// - Prefer pass-by-value + swap
// - Assignment DOES NOT construct; object already exists
// - Strong exception guarantee is expected

// ============================================================
// 4. STREAM INSERTION / EXTRACTION — DISCOVERABILITY
// ============================================================

struct User {
    std::string name;
    int id{};
};

// MUST be free functions
// MUST be in the same namespace for ADL
std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << "User{name=" << u.name << ", id=" << u.id << "}";
}

std::istream& operator>>(std::istream& is, User& u) {
    return is >> u.name >> u.id;
}

// Nuances:
// - Do NOT make these members
// - Return stream by reference
// - Do NOT print newlines
// - Formatting is part of API contract

// ============================================================
// 5. OPERATOR OVERLOADING CONTRACTS (WHAT USERS EXPECT)
// ============================================================

struct Counter {
    int value{0};

    Counter& operator++() {
        ++value;
        return *this;
    }

    Counter operator++(int) {
        Counter temp = *this;
        ++(*this);
        return temp;
    }
};

// Expected semantics:
// - Prefix returns mutated object
// - Postfix returns old value
// - Postfix is more expensive

// ============================================================
// 6. WHAT *NOT* TO OVERLOAD
// ============================================================

struct Dangerous {
    int x;

    // ❌ Violates expectations
    bool operator==(const Dangerous&) const {
        return false; // surprise!
    }

    // ❌ Overloading && / || breaks short-circuiting
    bool operator&&(const Dangerous&) const = delete;
};

// Rules of thumb:
// - Overload only when semantics are obvious
// - Operators should behave like built-in types
// - If you have to explain it, don’t overload it

// ============================================================
// 7. PERFORMANCE & ABI NOTES
// ============================================================
//
// - Prefer non-member operators for symmetry
// - Mark noexcept whenever possible
// - Inline small operators
// - Avoid temporaries in hot paths
// - Changing operator signatures is ABI-breaking
//
// ============================================================
// MAIN — LIGHT SANITY CHECKS
// ============================================================

int main() {
    Vec2 a{1,2}, b{3,4};
    Vec2 c = a + b;

    Version v1{1,2,3}, v2{1,3,0};
    assert(v1 < v2);

    Buffer buf1(10);
    Buffer buf2(5);
    buf2 = buf1;

    User u{"alice", 42};
    std::cout << u << '\n';

    Counter cnt;
    cnt++;
    ++cnt;

    return 0;
}
