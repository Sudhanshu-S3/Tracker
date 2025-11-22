
1. Memory Model , RAII , Ownership : 

Theory: 

Stack vs. Heap latency, CPU Cache lines (L1/L2), and why we hate new.

Lifetime, deterministic cleanup, resource ownership beyond memory (files, sockets).


Practice:

RAII guard: FileGuard that opens a file in ctor, closes in dtor; demonstrate exceptions don’t leak.

Leak detector: intentionally leak on the heap, catch with AddressSanitizer; then fix with RAII.

Scope & lifetime demo: make functions that return references/pointers to stack data (and see UB with UBSan).



### 1. Stack vs Heap memory

Stack :- 

Its is linear , contiguous , and extremely fast because it usually lives in the CPU's L1 Cache. Allocating here is just moving a pointer.

Headp :-

This is the slow warehouse. Asking for memory here (new or malloc) requires a "system call" to the OS, which pauses your program. It also causes fragementation.



### 2. RAII ( Resource Acquisition is Initialization )

The resource life ( memory , file handle, network socket ) to the life of a variable on the stack.
When the stack variable goes out of scope , C++ automatically destroys it. No garbage collector pauses.


### 3. RAII File Guard


### 4. Proof of Leak Without RAII


### 5. Modern Ownership (C++17/20)

We don't pass heavy objects around. We pass views or we move ownership.

Owner (std::unique_ptr, std::vector): "I own this memory. When I die, the memory is freed."

Non-Owner/View (std::span, std::string_view): "I am just looking at this memory. I don't own it." (Crucial for C++20)