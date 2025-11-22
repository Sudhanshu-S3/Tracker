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

### 

1. Memory Model & RAII: Addition: Stack vs. Heap latency, CPU Cache lines (L1/L2), and why we hate new.

2. Move Semantics: Addition: std::move is a lie (it's just a cast), and Return Value Optimization (RVO).

3. Smart Pointers: Constraint: Focus on std::unique_ptr. We rarely use std::shared_ptr in hot paths.

4. Exceptions: Note: Many HFT firms compile with -fno-exceptions. We will learn "Result types" (std::expected in C++23 or std::variant) as alternatives.

5. Templates (The Modern Way): Forget SFINAE complexity. We will use C++20 Concepts. It’s cleaner and faster to compile.

6. Concurrency: Addition: False Sharing and Hardware Memory Models.

7. Lock-Free: Addition: The ABA problem and Hazard Pointers.

8. Patterns: Addition: CRTP (Curiously Recurring Template Pattern) for static polymorphism (virtual functions without the v-table cost).

9. Allocators: std::pmr (Polymorphic Memory Resources) is mandatory learning for C++17/20.

10. Capstone: A lock-free Market Data Handler.