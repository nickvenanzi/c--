// day18_stl_containers_advanced.cpp
//
// Day 18: STL Containers — Deep Systems-Level Overview
//
// Topics:
// 1. Sequence containers (vector, deque, list)
// 2. Container adapters (stack, queue, priority_queue)
// 3. Associative containers (map, set)
// 4. Performance characteristics & iterator invalidation
//
// Compile:
//   g++ -std=c++20 day18_stl_containers_advanced.cpp -O2 -Wall -Wextra
//

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <string>
#include <chrono>
#include <algorithm>

// ============================================================
// 1. VECTOR — CONTIGUOUS DYNAMIC ARRAY
// ============================================================
//
// Internals:
// - Single contiguous heap allocation
// - Growth via geometric expansion (typically x2)
// - Maintains size and capacity
//
// Layout (conceptually):
// [ element0 | element1 | element2 | ... ]
//
// Key properties:
// - O(1) random access
// - Amortized O(1) push_back
// - O(n) insert/erase in middle
// - Cache friendly
//
// Iterator invalidation:
// - Reallocation invalidates ALL iterators
// - Erasing invalidates iterators after erase point
//

void vector_demo() {
    std::vector<int> v;
    v.reserve(8); // prevents early reallocations

    for (int i = 0; i < 5; ++i)
        v.push_back(i);

    std::cout << "Vector contents: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Size: " << v.size()
              << ", Capacity: " << v.capacity() << "\n";
}

// Why reserve matters:
void vector_growth_observation() {
    std::vector<int> v;

    size_t previous_capacity = v.capacity();
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
        if (v.capacity() != previous_capacity) {
            std::cout << "Capacity grew to: " << v.capacity() << "\n";
            previous_capacity = v.capacity();
        }
    }
}

// When to use vector:
// - Default choice 80% of time
// - Frequent iteration
// - Random access
// - Append-heavy workloads

// ============================================================
// 2. DEQUE — SEGMENTED ARRAY
// ============================================================
//
// Internals:
// - Multiple fixed-size blocks
// - Indirection table (map of block pointers)
//
// Layout (conceptually):
// [block][block][block]
//
// Advantages:
// - O(1) push_front and push_back
// - O(1) random access
//
// Tradeoffs:
// - Less cache friendly than vector
// - Slightly higher memory overhead
//

void deque_demo() {
    std::deque<int> d;

    d.push_back(1);
    d.push_front(0);
    d.push_back(2);

    std::cout << "Deque: ";
    for (int x : d)
        std::cout << x << " ";
    std::cout << "\n";
}

// Iterator invalidation:
// - Inserting at ends may invalidate iterators
// - Middle insert invalidates all iterators

// When to use deque:
// - Frequent push_front required
// - Random access needed
// - Avoid if vector suffices

// ============================================================
// 3. LIST — DOUBLY LINKED LIST
// ============================================================
//
// Internals:
// - Each element is separate heap allocation
// - Doubly linked nodes
//
// Layout:
// node <-> node <-> node
//
// Properties:
// - O(1) insert/erase anywhere given iterator
// - No random access
// - Poor cache locality
//
// Iterator invalidation:
// - Erase invalidates only erased iterator
// - Others remain valid
//

void list_demo() {
    std::list<int> lst = {1, 2, 3};

    auto it = std::next(lst.begin());
    lst.insert(it, 99);

    std::cout << "List: ";
    for (int x : lst)
        std::cout << x << " ";
    std::cout << "\n";
}

// Modern reality:
// - Rarely best choice
// - Often slower than vector even for inserts
// - Cache misses dominate theoretical O(1)

// ============================================================
// 4. CONTAINER ADAPTERS
// ============================================================
//
// Adapters wrap underlying containers.
//
// stack -> LIFO
// queue -> FIFO
// priority_queue -> heap
//
// Default underlying containers:
// - stack: deque
// - queue: deque
// - priority_queue: vector
//

void adapter_demo() {
    std::stack<int> s;
    s.push(1);
    s.push(2);
    std::cout << "Stack top: " << s.top() << "\n";

    std::queue<int> q;
    q.push(10);
    q.push(20);
    std::cout << "Queue front: " << q.front() << "\n";

    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(10);
    pq.push(5);
    std::cout << "Priority queue top: " << pq.top() << "\n"; // max-heap
}

// priority_queue internals:
// - Binary heap
// - push/pop O(log n)
// - top O(1)

// ============================================================
// 5. ASSOCIATIVE CONTAINERS (TREE-BASED)
// ============================================================
//
// map, set, multimap, multiset
//
// Internals:
// - Red-Black Tree (self-balancing BST)
//
// Properties:
// - Ordered
// - O(log n) insert, erase, find
// - Stable iterators (except erased element)
//

void map_set_demo() {
    std::map<std::string, int> m;
    m["Alice"] = 30;
    m["Bob"] = 25;

    std::cout << "Bob's age: " << m["Bob"] << "\n";

    std::set<int> s = {3, 1, 2};
    std::cout << "Set contents (ordered): ";
    for (int x : s)
        std::cout << x << " ";
    std::cout << "\n";
}

// Key insight:
// - Always sorted
// - Memory heavy (node-based)
// - Many small allocations

// ============================================================
// 6. PERFORMANCE CHARACTERISTICS SUMMARY
// ============================================================
//
// Operation        vector     deque     list      map/set
// ------------------------------------------------------------
// Random access    O(1)       O(1)      O(n)      O(log n)
// push_back        Amort O(1) O(1)      O(1)      O(log n)
// push_front       O(n)       O(1)      O(1)      O(log n)
// insert middle    O(n)       O(n)      O(1)*     O(log n)
// find             O(n)       O(n)      O(n)      O(log n)
//
// *list requires iterator
//
// Real-world rule:
// - vector beats list most of the time
// - tree containers are slower than unordered_map for lookups
//

// ============================================================
// 7. MEMORY & CACHE BEHAVIOR
// ============================================================
//
// vector:
// - Best spatial locality
// - Prefetch friendly
//
// deque:
// - Slight indirection penalty
//
// list:
// - Pointer chasing
// - Cache miss heavy
//
// map/set:
// - Tree traversal
// - Poor locality
//

// ============================================================
// 8. ADVANCED NOTES
// ============================================================
//
// - Emplace avoids temporary construction
// - reserve() prevents repeated reallocation
// - shrink_to_fit is non-binding
// - erase-remove idiom for vector
// - stable_sort vs sort
//
// Always measure. Big-O ≠ real-world performance.
//

// ============================================================
// MAIN
// ============================================================

int main() {

    std::cout << "\n--- Vector ---\n";
    vector_demo();

    std::cout << "\n--- Vector Growth ---\n";
    vector_growth_observation();

    std::cout << "\n--- Deque ---\n";
    deque_demo();

    std::cout << "\n--- List ---\n";
    list_demo();

    std::cout << "\n--- Adapters ---\n";
    adapter_demo();

    std::cout << "\n--- Map/Set ---\n";
    map_set_demo();

    return 0;
}
