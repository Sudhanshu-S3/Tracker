# Advanced C++ Plan

Focus topics:
- Memory ownership, RAII, and smart pointers
- Move semantics, rule of five, and template mastery
- Concurrency primitives and lock-free patterns
- STL internals, allocators, and compile-time tricks

Hands-on goals:
- Write a thread pool and a bounded queue
- Build a custom allocator and experiment with SFINAE guards
- Profile code paths for latency and cache behavior


### Path (ordered)

1. Memory model, RAII & ownership (foundation)
2. Rule of Five & move semantics (copy/move, elision)
3. Smart pointers & custom deleters (practical ownership)
4. Exceptions & exception safety guarantees
5. Templates I: basics → SFINAE → Concepts → CRTP → small meta tricks
6. Concurrency I: threads, mutexes, condition variables
7. Concurrency II: atomics, memory order, lock-free intro
8. Patterns in practice: thread-safe singleton (correctly), template functions with type checking
9. Custom allocators (std::pmr and a simple pool)
10. Capstone: tie it all together in a small, concurrent, generic library


