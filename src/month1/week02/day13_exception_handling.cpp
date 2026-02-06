// day13_exception_handling_advanced.cpp
//
// Day 13 (Advanced): Exception Handling — Control Flow Under Failure
//
// Assumptions:
// - You know try/catch/throw syntax
// - You care about correctness under partial execution
//
// Topics:
// 1. try / catch / throw mechanics
// 2. Exception safety guarantees
// 3. RAII (Resource Acquisition Is Initialization)
// 4. Performance cost of exceptions
//
// Compile with:
//   g++ -std=c++20 day13_exception_handling_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>

// ============================================================
// 1. THROW MECHANICS — WHAT ACTUALLY HAPPENS
// ============================================================

struct Tracked {
    Tracked()  { std::cout << "Tracked constructed\n"; }
    ~Tracked() { std::cout << "Tracked destroyed\n"; }
};

void thrower() {
    Tracked t;
    throw std::runtime_error("boom");
    // Unreachable
}

void catcher() {
    try {
        thrower();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }
}

// Key mechanics:
// - Stack unwinding destroys all fully-constructed objects
// - Destructors are called in reverse construction order
// - Catch by const reference (avoid slicing + copies)

// ============================================================
// 2. CATCHING — TYPE MATCHING & ORDER MATTERS
// ============================================================

struct BaseError : std::exception {
    const char* what() const noexcept override {
        return "BaseError";
    }
};

struct DerivedError : BaseError {
    const char* what() const noexcept override {
        return "DerivedError";
    }
};

void throw_derived() {
    throw DerivedError{};
}

void catch_order() {
    try {
        throw_derived();
    }
    catch (const DerivedError&) {
        std::cout << "Caught Derived\n";
    }
    catch (const BaseError&) {
        std::cout << "Caught Base\n";
    }
}

// Order matters:
// - First matching handler wins
// - Catch derived before base

// ============================================================
// 3. EXCEPTION SAFETY GUARANTEES
// ============================================================
//
// No-throw guarantee:
//   - Operation never throws
// Strong guarantee:
//   - Either succeeds completely or has no effect
// Basic guarantee:
//   - Invariants preserved, state may change
// No guarantee:
//   - All bets off

struct StrongGuarantee {
    std::vector<int> data;

    void push(int v) {
        // std::vector::push_back gives strong guarantee
        data.push_back(v);
    }
};

struct BasicGuarantee {
    std::vector<int> data;

    void push(int v) {
        data.reserve(data.size() + 1); // may throw
        data.push_back(v);
    }
};

// Subtlety:
// - Strong guarantee often implemented via copy/swap
// - Strong ≠ cheap

// ============================================================
// 4. RAII — THE ONLY REAL EXCEPTION SAFETY TOOL
// ============================================================

struct File {
    FILE* f{nullptr};

    explicit File(const char* path) {
        f = std::fopen(path, "w");
        if (!f) throw std::runtime_error("file open failed");
    }

    ~File() {
        if (f) std::fclose(f);
    }

    // Non-copyable, movable
    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&& other) noexcept : f(other.f) {
        other.f = nullptr;
    }
};

// RAII rule:
// - Acquire in constructor
// - Release in destructor
// - Destructor MUST NOT throw

// ============================================================
// 5. EXCEPTION SAFETY VIA COMPOSITION
// ============================================================

struct Resource {
    Resource() {
        std::cout << "Resource acquired\n";
    }
    ~Resource() {
        std::cout << "Resource released\n";
    }
};

struct Owner {
    Resource r1;
    Resource r2;

    Owner() {
        // If r2 throws, r1 is cleaned up automatically
    }
};

// No try/catch needed
// No manual cleanup
// This is why RAII works

// ============================================================
// 6. DESTRUCTORS AND EXCEPTIONS
// ============================================================

struct BadDestructor {
    ~BadDestructor() {
        // NEVER DO THIS
        // If this runs during stack unwinding → std::terminate
        // throw std::runtime_error("oops");
    }
};

// Rule:
// - Destructors are implicitly noexcept(true)
// - Violating this terminates the program

// ============================================================
// 7. PERFORMANCE COST MODEL OF EXCEPTIONS
// ============================================================
//
// Zero-cost model (Itanium ABI, most platforms):
// - try/catch adds *no cost* in non-throwing path
// - Throwing is VERY expensive
//
// Costs when throwing:
// - Stack unwinding
// - RTTI lookup
// - Cache misses
// - Destructor calls
//
// Implications:
// - Exceptions are for exceptional paths
// - Not for control flow
// - Error codes may be better in hot paths

void no_throw_path() {
    int x = 0;
    x += 42; // no overhead from try/catch
}

void hot_path(bool fail) {
    if (fail) {
        throw std::runtime_error("slow path");
    }
}

// ============================================================
// 8. WHEN *NOT* TO USE EXCEPTIONS
// ============================================================
//
// - Real-time systems
// - Kernel / embedded environments
// - Tight inner loops
// - ABI boundaries where exceptions are banned
//
// Alternatives:
// - std::optional
// - std::expected (C++23)
// - Error codes
//
// ============================================================
// MAIN — OBSERVE UNWINDING
// ============================================================

int main() {
    std::cout << "\n--- Stack Unwinding ---\n";
    catcher();

    std::cout << "\n--- Catch Order ---\n";
    catch_order();

    std::cout << "\n--- RAII Cleanup ---\n";
    try {
        File f("example.txt");
        throw std::runtime_error("write failed");
    } catch (...) {
        std::cout << "Exception caught, file closed\n";
    }

    std::cout << "\n--- Composition Cleanup ---\n";
    try {
        Owner o;
        throw std::runtime_error("fail");
    } catch (...) {
        std::cout << "Owner cleaned up\n";
    }

    return 0;
}
