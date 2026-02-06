// day9_constructors_advanced.cpp
//
// Day 9 (Advanced): Constructors and Initialization Nuances
//
// Assumptions:
// - You already understand what constructors are
// - You care about object lifetime, ABI, performance, and rules-lawyering
//
// Topics:
// 1. Default / parameterized / copy constructors (implicit generation rules)
// 2. Member initializer lists (semantic + performance guarantees)
// 3. Constructor delegation (ordering, pitfalls, base/member behavior)
// 4. Initialization vs assignment (observable effects, temporaries, elision)
//
// Compile with:
//   g++ -std=c++20 day9_constructors_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <string>
#include <vector>
#include <utility>

// ============================================================
// 1. CONSTRUCTOR GENERATION RULES (NOT JUST SYNTAX)
// ============================================================

struct Trivial {
    int x;
    // No constructors declared:
    // - Default constructor is implicitly declared
    // - Copy/move constructors are trivial
    // - Object is trivially constructible
};

struct NonTrivial {
    int x;

    // User-declared constructor:
    NonTrivial(int v) : x(v) {}

    // Consequences:
    // - Default constructor is NOT generated
    // - Copy constructor is still generated
    // - Type is no longer an aggregate
};

// Uncommenting this changes everything:
// NonTrivial() = default;

struct CopyObserved {
    std::string s;

    CopyObserved(const std::string& str) : s(str) {}

    CopyObserved(const CopyObserved& other)
        : s(other.s)
    {
        std::cout << "Copy ctor\n";
    }
};

// Key nuance:
// - A user-declared copy ctor suppresses implicit move ctor
// - You *must* explicitly =default move if you want it

// ============================================================
// 2. MEMBER INITIALIZER LISTS — SEMANTICS, NOT STYLE
// ============================================================

struct InitOrder {
    int a;
    int b;

    InitOrder()
        : b(2), a(1) // WARNING: order here is IGNORED
    {
        // Actual order:
        //   a initialized first
        //   b initialized second
    }
};

// Initialization order is determined by **member declaration order**
// NOT initializer-list order. This is a correctness issue, not cosmetic.

struct RequiresInitList {
    const int c;
    int& ref;

    RequiresInitList(int& r)
        : c(42), ref(r)
    {
        // Without initializer list:
        // ❌ ill-formed, not just inefficient
    }
};

// ============================================================
// 3. INITIALIZATION VS ASSIGNMENT — OBSERVABLE DIFFERENCES
// ============================================================

struct Instrumented {
    Instrumented() {
        std::cout << "Default ctor\n";
    }

    Instrumented(int) {
        std::cout << "Value ctor\n";
    }

    Instrumented(const Instrumented&) {
        std::cout << "Copy ctor\n";
    }

    Instrumented& operator=(const Instrumented&) {
        std::cout << "Copy assignment\n";
        return *this;
    }
};

struct AssignmentPath {
    Instrumented i;

    AssignmentPath() {
        i = Instrumented(1);
        // Observed:
        // Default ctor
        // Value ctor
        // Copy assignment
    }
};

struct InitializationPath {
    Instrumented i;

    InitializationPath()
        : i(1)
    {
        // Observed:
        // Value ctor
    }
};

// This difference matters when:
// - ctor allocates
// - assignment must clean up existing state
// - exception safety differs
// - atomic or RAII semantics exist

// ============================================================
// 4. CONSTRUCTOR DELEGATION (C++11+) — REAL RULES
// ============================================================

struct Delegation {
    int x;
    std::vector<int> v;

    Delegation(int n)
        : x(n), v(n, 0)
    {
        std::cout << "Primary ctor\n";
    }

    Delegation()
        : Delegation(10) // delegates fully
    {
        std::cout << "Delegating ctor body\n";
    }
};

// Important guarantees:
// - Delegating constructor does NOT initialize members itself
// - Delegated-to constructor initializes *everything*
// - Base classes are initialized by the target constructor only

// Forbidden:
// Delegation() : x(0), Delegation(10) {} // ❌ ill-formed

// ============================================================
// 5. COPY ELISION & CONSTRUCTORS (WHAT YOU CAN OBSERVE)
// ============================================================

CopyObserved make_obj() {
    CopyObserved tmp("hello");
    return tmp; // guaranteed copy elision (C++17)
}

void pass_by_value(CopyObserved obj) {
    (void)obj;
}

// ============================================================
// 6. SUBTLE PITFALL: INITIALIZATION LOOKS LIKE ASSIGNMENT
// ============================================================

struct LooksLikeAssignment {
    std::string s;

    LooksLikeAssignment(const char* c)
        : s(c) // construction
    {}

    LooksLikeAssignment()
    {
        s = "assigned"; // assignment after default construction
    }
};

// These are NOT equivalent:
//   LooksLikeAssignment a("hi");
//   LooksLikeAssignment b; b.s = "hi";

// ============================================================
// 7. BASE CLASSES AND INITIALIZATION GUARANTEES
// ============================================================

struct Base {
    Base(int) {
        std::cout << "Base ctor\n";
    }
};

struct Derived : Base {
    int x;

    Derived()
        : Base(1), x(42)
    {
        // Base is ALWAYS constructed before members
        // Members before body
    }
};

// ============================================================
// MAIN — OBSERVE CONSTRUCTION SEQUENCES
// ============================================================

int main() {
    std::cout << "\n--- Initialization vs Assignment ---\n";
    AssignmentPath a;
    InitializationPath b;

    std::cout << "\n--- Delegation ---\n";
    Delegation d;

    std::cout << "\n--- Copy Elision ---\n";
    CopyObserved c = make_obj();

    std::cout << "\n--- Pass by Value ---\n";
    pass_by_value(c);

    return 0;
}
