1) memory model, RAII & ownership

you’ll grok: stack vs heap, lifetime, deterministic cleanup, resource ownership beyond memory (files, sockets).
labs:

RAII guard: FileGuard that opens a file in ctor, closes in dtor; demonstrate exceptions don’t leak.

Leak detector: intentionally leak on the heap, catch with AddressSanitizer; then fix with RAII.

Scope & lifetime demo: make functions that return references/pointers to stack data (and see UB with UBSan).



### 1. Stack vs Heap memory




### 2. RAII


### 3. RAII File Guard


### 4. Proof of Leak Without RAII