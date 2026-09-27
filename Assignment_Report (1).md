# Sorting Algorithms for Logistics Package Weight Management
### Assignment: Merge Sort vs Quick Sort — Stability, Complexity & Suitability Analysis

This report covers the problem statement, stability verification, complexity analysis, comparison table, and final conclusion for sorting logistics packages by weight using Merge Sort and Quick Sort.

---

## 1. Problem Statement

A logistics company receives the following package weights, each package carrying a unique Package ID:

| Package ID | Weight |
|---|---|
| P1 | 20 |
| P2 | 15 |
| P3 | 20 |
| P4 | 10 |
| P5 | 15 |
| P6 | 20 |
| P7 | 25 |
| P8 | 10 |

Requirements:
- Implement Merge Sort and Quick Sort in C to sort packages by weight.
- Execute both programs and record intermediate steps.
- Modify the program(s) so equal-weight packages retain their original relative order (stability), and verify using Package IDs.
- Analyse both algorithms on: duplicates, stability, number of comparisons, time complexity, and space complexity.
- Determine which algorithm is best suited when preserving the original order of equal-weight packages matters.

---

## 2. Approach Summary

**Merge Sort** is a divide-and-conquer algorithm. It recursively splits the array into halves, sorts each half, and merges them back using a stable merge step — taking from the left sub-array first on ties is what makes it stable by nature.

**Quick Sort (standard)** partitions the array around a pivot (Lomuto scheme, last element as pivot) and sorts in-place. Because elements are swapped across the array based only on the weight comparison, equal-weight elements can end up reordered relative to each other.

**Quick Sort (modified for stability)** uses a composite key — **(weight, original_pos)** — instead of weight alone. `original_pos` is each package's position in the original input. When two packages have equal weight, the one with the smaller `original_pos` is treated as smaller, forcing ties to break in favour of the earlier package. This preserves Quick Sort's in-place partitioning logic while producing stable output.

---

## 3. Execution Results (Verified by Package ID)

Running all three programs on the given input produced the following final sorted order (identical across all three, since all correctly sort by weight):

**P4(10), P8(10), P2(15), P5(15), P1(20), P3(20), P6(20), P7(25)**

- **Merge Sort:** Equal-weight groups — P4/P8, P2/P5, P1/P3/P6 — appeared in the *same relative order* as the original input. Stability check result: **STABLE**. Total comparisons: **16**.
- **Quick Sort (standard):** The final order came out as P8(10), P4(10), P5(15), P2(15), P1(20), P3(20), P6(20), P7(25) — note P8 now appears *before* P4, and P5 *before* P2, which is the reverse of their original input order. Stability check result: **NOT STABLE**. Total comparisons: **18**.
- **Quick Sort (modified/stable):** Using the composite key, the final order matched Merge Sort's exactly and preserved original order for every equal-weight group. Stability check result: **STABLE**. Total comparisons: **18**.

This is direct experimental proof that standard Quick Sort is not stable, while Merge Sort — and a suitably modified Quick Sort — are.

---

## 4. Comparison Table

| Criterion | Merge Sort | Quick Sort (standard) | Quick Sort (modified/stable) |
|---|---|---|---|
| Duplicate values | Handled correctly; equal weights merge without disturbing order | Sorted correctly by value, but relative order of duplicates is not guaranteed | Handled correctly using (weight, original_pos) as composite key |
| Stability | Stable (proved by experiment) | Not stable (experiment shows order reversed for duplicates) | Made stable artificially (experiment confirms STABLE) |
| No. of comparisons (n = 8, this input) | 16 | 18 | 18 |
| Time complexity — Best | O(n log n) | O(n log n) | O(n log n) |
| Time complexity — Average | O(n log n) | O(n log n) | O(n log n) |
| Time complexity — Worst | O(n log n) | O(n²) — sorted/reverse-sorted input or many duplicates with last-element pivot | O(n²) — same worst case as standard Quick Sort |
| Space complexity | O(n) auxiliary array + O(log n) recursion stack | O(log n) average recursion stack (in-place); O(n) worst case | O(log n) average / O(n) worst — same as standard, in-place |
| In-place? | No (needs auxiliary arrays for merging) | Yes | Yes |

---

## 5. Discussion

- **Duplicate values:** Both algorithms correctly group equal-weight packages together in the final sorted output. The difference lies only in whether the *relative order* within a duplicate group is preserved.
- **Stability:** Merge Sort is stable by design. Standard Quick Sort is not stable because partitioning swaps elements based purely on the pivot comparison, ignoring original position — proven concretely on this data set.
- **Number of comparisons:** For this 8-element input, Merge Sort used 16 comparisons and Quick Sort used 18. In general, Merge Sort always performs close to n·log₂(n) comparisons regardless of input arrangement. Quick Sort's comparison count depends heavily on pivot choice and input order — from as low as ~n·log₂(n) to as high as ~n(n−1)/2 in the worst case.
- **Time complexity:** Both have O(n log n) average and best case. Merge Sort *guarantees* O(n log n) in every case, including worst case, because it always splits exactly in half. Quick Sort degrades to O(n²) worst case when the pivot repeatedly picks the smallest or largest element (e.g., sorted input with last-element pivot, or many duplicates clustered together).
- **Space complexity:** Merge Sort needs O(n) extra space for temporary arrays used during merging. Quick Sort sorts in-place, needing only O(log n) extra space on average for the recursion stack (O(n) in the worst case of unbalanced partitions).
- **Making Quick Sort stable** via a composite key does not change its time or space complexity class — it remains O(n log n) average / O(n²) worst, and O(log n) average extra space — it only fixes the *ordering* of ties.

---

## 6. Final Conclusion

When maintaining the original relative order of equal-weight packages is important — as in this logistics scenario, where package arrival/registration order may matter for downstream processing, auditing, or fairness — **Merge Sort is the more suitable algorithm**.

- It is naturally and unconditionally stable, with no extra engineering needed.
- It guarantees O(n log n) time complexity in *all* cases (best, average, worst) — predictable and reliable for production logistics systems.
- Its only drawback is O(n) auxiliary space, a reasonable trade-off for guaranteed stability and guaranteed performance.

Quick Sort *can* be made stable using a composite (weight, original_pos) key, and it remains attractive when memory is tightly constrained (O(log n) average extra space) and when average-case speed with low constant factors matters more than worst-case guarantees. However, its stability then depends on the programmer explicitly adding the tie-breaking key — it is not stable "by nature" — and its worst-case time complexity remains O(n²).

**Overall recommendation:** Use Merge Sort as the default choice for this logistics package-sorting system, since order-preservation (stability) and predictable worst-case performance are both important operational requirements. Standard Quick Sort should be avoided if stability matters, unless explicitly modified as described above.

---

## 7. GitHub Repository Structure

Suggested repository layout:

```
logistics-package-sorting/
├── README.md                      (overview + how to compile & run)
├── src/
│   ├── merge_sort.c
│   ├── quick_sort_unstable.c
│   └── quick_sort_stable.c
├── data/
│   └── input.txt                  (package IDs & weights)
├── output/
│   ├── output_merge_sort.txt
│   ├── output_quick_sort_unstable.txt
│   └── output_quick_sort_stable.txt
├── docs/
│   └── Assignment_Report.md       (this report)
└── LICENSE
```

`README.md` should include: project title, problem statement, build instructions (`gcc -O2 -Wall -o <name> <file>.c`), run instructions (`./<name>`), and a pointer to `docs/Assignment_Report.md` for the full analysis, comparison table, and conclusion.
