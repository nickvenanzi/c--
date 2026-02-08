// day19_stl_iterators_advanced.cpp
//
// Day 19: STL Iterators — Deep Dive
//
// Topics:
// 1. Iterator categories and hierarchy
// 2. Iterator arithmetic & performance implications
// 3. Tag dispatch & algorithm specialization
// 4. Custom iterator implementation
// 5. Iterator invalidation rules
//
// Compile:
//   g++ -std=c++20 day19_stl_iterators_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <type_traits>
#include <cassert>

// ============================================================
// 1. ITERATOR CATEGORY HIERARCHY
// ============================================================
//
// InputIterator
//   ↓
// ForwardIterator
//   ↓
// BidirectionalIterator
//   ↓
// RandomAccessIterator
//   ↓
// ContiguousIterator (C++20)
//
// Each level strictly adds capabilities.
//
// Example mapping:
// - vector<T>::iterator → random access + contiguous
// - deque<T>::iterator  → random access
// - list<T>::iterator   → bidirectional
// - forward_list        → forward
//

void category_demo() {
    using VecIt = std::vector<int>::iterator;
    using ListIt = std::list<int>::iterator;

    static_assert(std::random_access_iterator<VecIt>);
    static_assert(std::bidirectional_iterator<ListIt>);
}

// ============================================================
// 2. ITERATOR ARITHMETIC & PERFORMANCE
// ============================================================
//
// Random access iterator:
//   it + n
//   it[n]
//   it2 - it1
//
// Bidirectional:
//   ++it, --it only
//
// Forward:
//   ++it only
//
// Input:
//   single-pass
//
// Performance impact:
// std::advance behaves differently depending on category.
//

template<typename Iterator>
void advance_n(Iterator& it, int n) {
    std::advance(it, n);
}

// Under the hood:
// - random access → constant time
// - bidirectional → linear time
// - forward → linear
//
// Example:
void advance_cost_demo() {
    std::vector<int> v(100000);
    auto it = v.begin();
    std::advance(it, 50000); // O(1)

    std::list<int> l(100000);
    auto lit = l.begin();
    std::advance(lit, 50000); // O(n)
}

// This is why algorithm performance depends on iterator category,
// not just container type.

// ============================================================
// 3. TAG DISPATCH (HOW ALGORITHMS SPECIALIZE)
// ============================================================

template<typename Iterator>
void distance_impl(Iterator first, Iterator last,
                   std::input_iterator_tag) {
    std::cout << "Using input iterator strategy\n";
    size_t count = 0;
    while (first != last) {
        ++first;
        ++count;
    }
    std::cout << "Distance: " << count << "\n";
}

template<typename Iterator>
void distance_impl(Iterator first, Iterator last,
                   std::random_access_iterator_tag) {
    std::cout << "Using random access strategy\n";
    std::cout << "Distance: " << (last - first) << "\n";
}

template<typename Iterator>
void my_distance(Iterator first, Iterator last) {
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    distance_impl(first, last, Category{});
}

// This is how STL algorithms adapt behavior
// based on iterator capabilities.

// ============================================================
// 4. CUSTOM ITERATOR IMPLEMENTATION
// ============================================================
//
// Minimal forward iterator example.
//

template<typename T>
class SimpleArray {
public:
    SimpleArray(size_t size)
        : size_(size), data_(new T[size]) {}

    ~SimpleArray() { delete[] data_; }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    size_t size() const { return size_; }

    // Forward iterator
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(T* ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.ptr_ == b.ptr_;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return !(a == b);
        }

    private:
        T* ptr_;
    };

    Iterator begin() { return Iterator(data_); }
    Iterator end()   { return Iterator(data_ + size_); }

private:
    size_t size_;
    T* data_;
};

void custom_iterator_demo() {
    SimpleArray<int> arr(5);
    for (size_t i = 0; i < arr.size(); ++i)
        arr[i] = static_cast<int>(i * 10);

    for (auto x : arr)
        std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 5. CONTIGUOUS ITERATORS (C++20)
// ============================================================
//
// vector, array, span
//
// Guarantee:
// - &*(it) gives contiguous memory
// - Enables memcpy optimizations
//

void contiguous_demo() {
    std::vector<int> v = {1,2,3};
    static_assert(std::contiguous_iterator<decltype(v.begin())>);
}

// ============================================================
// 6. ITERATOR INVALIDATION RULES
// ============================================================
//
// VECTOR
// - Reallocation → all invalid
// - Erase → iterators after erase invalid
//
// DEQUE
// - Insert middle → all invalid
// - Ends → may invalidate
//
// LIST
// - Only erased element invalid
//
// MAP/SET
// - Only erased element invalid
//
// UNORDERED
// - Rehash → all invalid
//

void invalidation_demo() {
    std::vector<int> v = {1,2,3};
    auto it = v.begin();

    v.push_back(4); // may reallocate
    // 'it' may now be invalid

    // NEVER use old iterator after potential reallocation.
}

// ============================================================
// 7. COMMON PITFALLS
// ============================================================
//
// 1. Assuming random access where only forward exists
// 2. Using invalidated iterators
// 3. Storing iterators across container mutations
// 4. Confusing iterator difference complexity
//
// Example bug:
//
// for (auto it = vec.begin(); it != vec.end(); ++it)
//     vec.push_back(...); // UB
//

// ============================================================
// MAIN
// ============================================================

int main() {

    std::cout << "\n--- Category Demo ---\n";
    category_demo();

    std::cout << "\n--- Advance Cost Demo ---\n";
    advance_cost_demo();

    std::cout << "\n--- Tag Dispatch Demo ---\n";
    std::vector<int> v(10);
    my_distance(v.begin(), v.end());

    std::list<int> l(10);
    my_distance(l.begin(), l.end());

    std::cout << "\n--- Custom Iterator Demo ---\n";
    custom_iterator_demo();

    std::cout << "\n--- Contiguous Iterator Demo ---\n";
    contiguous_demo();

    return 0;
}
