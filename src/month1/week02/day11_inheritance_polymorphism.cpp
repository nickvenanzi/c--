// day11_inheritance_and_polymorphism_advanced.cpp
//
// Day 11 (Advanced): Inheritance & Polymorphism — Object Model Reality
//
// Assumptions:
// - You understand inheritance syntax
// - You want to understand what the compiler *actually* builds
//
// Topics:
// 1. Public / protected / private inheritance
// 2. Virtual functions and vtables
// 3. Pure virtual functions and abstract classes
// 4. Virtual destructor necessity
//
// Compile with:
//   g++ -std=c++20 day11_inheritance_and_polymorphism_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <memory>
#include <cassert>

// ============================================================
// 1. INHERITANCE VISIBILITY — IS-A VS IMPLEMENTS-IN-TERMS-OF
// ============================================================

struct Interface {
    virtual void f() = 0;
};

struct PublicDerived : public Interface {
    void f() override {}
};

// Public inheritance:
// - Models IS-A relationship
// - Enables polymorphism
// - Required for substitutability (LSP)

struct Helper {
    void helper() {}
};

struct PrivateDerived : private Helper {
    void do_work() {
        helper(); // allowed
    }
};

// Private inheritance:
// - NOT an IS-A relationship
// - "Implemented in terms of"
// - Similar to composition, but with access to protected members
// - No implicit upcasting

struct ProtectedDerived : protected Helper {
    // Rare
    // Used mainly for CRTP or framework internals
};

// ============================================================
// 2. VIRTUAL FUNCTIONS — WHAT THE VTABLE REALLY MEANS
// ============================================================

struct Base {
    virtual void foo() {
        std::cout << "Base::foo\n";
    }

    void bar() {
        std::cout << "Base::bar\n";
    }
};

struct Derived : Base {
    void foo() override {
        std::cout << "Derived::foo\n";
    }

    void bar() { // NOT virtual
        std::cout << "Derived::bar\n";
    }
};

// Key rules:
// - Virtual dispatch happens ONLY through base-class references/pointers
// - Non-virtual functions are statically bound
// - "override" is compile-time only (no runtime effect)

void call(Base& b) {
    b.foo(); // virtual
    b.bar(); // static
}

// VTABLE FACTS (typical implementation):
// - One vptr per polymorphic subobject
// - vptr set during construction
// - Virtual calls are indirect calls (cannot inline easily)
// - Devirtualization may occur if type is known

// ============================================================
// 3. OBJECT LIFETIME DURING CONSTRUCTION / DESTRUCTION
// ============================================================

struct LifetimeBase {
    LifetimeBase() {
        call_virtual();
    }

    virtual ~LifetimeBase() = default;

    virtual void call_virtual() {
        std::cout << "LifetimeBase::call_virtual\n";
    }
};

struct LifetimeDerived : LifetimeBase {
    LifetimeDerived() {
        call_virtual();
    }

    void call_virtual() override {
        std::cout << "LifetimeDerived::call_virtual\n";
    }
};

// Observed behavior:
// - Base constructor calls Base version
// - Derived constructor calls Derived version
// - During construction, object is treated as current base

// ============================================================
// 4. PURE VIRTUAL FUNCTIONS & ABSTRACT CLASSES
// ============================================================

struct Shape {
    virtual double area() const = 0; // pure virtual

    virtual ~Shape() = default;
};

// Abstract class:
// - Cannot be instantiated
// - Can have data members
// - Can have implemented virtual functions

struct Rectangle : Shape {
    double w, h;

    Rectangle(double w, double h) : w(w), h(h) {}

    double area() const override {
        return w * h;
    }
};

// Nuance:
// - Pure virtual functions CAN have definitions
// - Still must be overridden to instantiate

// ============================================================
// 5. VIRTUAL DESTRUCTORS — WHY THEY ARE REQUIRED
// ============================================================

struct BadBase {
    ~BadBase() {
        std::cout << "BadBase destroyed\n";
    }
};

struct BadDerived : BadBase {
    ~BadDerived() {
        std::cout << "BadDerived destroyed\n";
    }
};

struct GoodBase {
    virtual ~GoodBase() {
        std::cout << "GoodBase destroyed\n";
    }
};

struct GoodDerived : GoodBase {
    ~GoodDerived() {
        std::cout << "GoodDerived destroyed\n";
    }
};

// Rule:
// - If a class is intended to be deleted polymorphically,
//   its destructor MUST be virtual

// ============================================================
// 6. OBJECT SLICING — SILENTLY LOSING DYNAMIC TYPE
// ============================================================

struct Animal {
    virtual const char* sound() const {
        return "???";
    }
};

struct Dog : Animal {
    const char* sound() const override {
        return "woof";
    }
};

void by_value(Animal a) {
    // Dog sliced to Animal
    std::cout << a.sound() << '\n';
}

void by_reference(const Animal& a) {
    std::cout << a.sound() << '\n';
}

// ============================================================
// 7. POLYMORPHISM VS ALTERNATIVES
// ============================================================
//
// Polymorphism costs:
// - vptr per object
// - Indirect calls
// - ABI fragility
//
// Alternatives:
// - std::variant + visit
// - Templates / static polymorphism
// - Function objects
//
// Use inheritance when:
// - You need runtime substitution
// - Stable interface matters
// - Object lifetimes are managed via base pointers

// ============================================================
// MAIN — OBSERVATION POINTS
// ============================================================

int main() {
    std::cout << "\n--- Virtual Dispatch ---\n";
    Derived d;
    call(d);

    std::cout << "\n--- Construction Order ---\n";
    LifetimeDerived ld;

    std::cout << "\n--- Polymorphic Destruction ---\n";
    {
        BadBase* b = new BadDerived();
        delete b; // UB: derived destructor not called
    }

    {
        GoodBase* g = new GoodDerived();
        delete g; // OK
    }

    std::cout << "\n--- Object Slicing ---\n";
    Dog dog;
    by_value(dog);
    by_reference(dog);

    std::cout << "\n--- Abstract Base ---\n";
    std::unique_ptr<Shape> s = std::make_unique<Rectangle>(3, 4);
    std::cout << s->area() << '\n';

    
    return 0;
}
