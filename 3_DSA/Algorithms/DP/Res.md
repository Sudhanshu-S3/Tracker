## Dynamic Programming

Practice both standard and non-standard DP problems to build confidence.

### How to build intuition
- Restate the problem in your own words and list the inputs and outputs.
- Try a small brute-force recursion to see overlapping subproblems.
- Identify the state (indexes, sum, mask, etc.) and write the recurrence.
- Draw a table or graph to visualize transitions.
- Memoize first, then convert to tabulation when it feels natural.
- Test with tiny cases and edge cases to check the state choices.

> Note: I will try to implement both Memoized and Tabulation Solution for every problem i practice.

Recommended sets:
1. CSES DP section – https://cses.fi/problemset/
2. AtCoder DP set – https://atcoder.jp/contests/dp/tasks



### DP is about

- State isolation ability — can you formalize subproblems cleanly.
- Transition inference — can you logically derive relation, not recall it.
- Constraint-driven optimization — can you compress space/time under pressure.

So before coding, the “mental dry run” in notebook form is exactly what calibrates intuition.


---

### Step you can follow 

---

#### 1. **State Discovery Phase (Notebook Work)**

Write down:

* What is the *goal quantity* (min, max, count, bool)?
* What minimal information defines a *partial solution*?
* Express one subproblem as `dp[i][j][k]...` in a **single clear sentence**.

If you can’t do that precisely, don’t proceed to transitions — your state is still malformed.

---

#### 2. **Overlap Identification**

Ask:

* “If I solve smaller instances, do they recur when solving larger ones?”
  → If yes, that’s overlapping substructure.
* Visualize recursion tree; identical nodes = overlapping subproblems.

Mark repeated states on paper — that’s the caching target.

---

#### 3. **Transition Derivation**

For each state, enumerate minimal decisions that move to next states.
Write recurrence *symbolically* first:

```
dp[x] = min/max/sum over { transition(dp[y], cost) }
```

Don’t assign loops or indices yet — that’s implementation detail.

---

#### 4. **Boundary and Order**

* Identify base cases where subproblem collapses trivially.
* Note dependency direction → determines iteration order (bottom-up or top-down).

---

#### 5. **Compression and Constraints Awareness**

* If dimensions are independent, consider rolling arrays or one-dimensional flattening.
* Rubrik problems often include N ≤ 10^5 with DP constraints → check feasibility early.

---

#### 6. **Pattern Categorization (build mental library)**

After each solved question, tag it by pattern in your notes:
`[counting] [knapsack-type] [interval] [bitmask] [digit] [treeDP]`
This builds recall pathways for unfamiliar problems later.

---

Your current strategy — *write, reason, then code* — is the professional workflow.
Next logical step: pick one easy DP (Dice Combinations or AtCoder Frog 1), and I’ll walk you through how to extract state + overlap formally on paper before coding.
Proceed with which one?
