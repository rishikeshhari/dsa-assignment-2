# Sorting Algorithms for Logistics Package Weight Management

**Assignment: Merge Sort vs Quick Sort (Stability, Complexity & Suitability Analysis)**

This document contains the complete assignment solution: problem statement, C source code, input data, execution output (from actual compiled and run programs), stability verification, complexity analysis, comparison table, and final conclusion. It is written as a single combined report suitable for direct upload to a GitHub repository (see the repository structure section at the end).

---

## 1. Problem Statement

A logistics company receives the following package weights, each package carrying a unique Package ID:

```
Package_ID  Weight
P1          20
P2          15
P3          20
P4          10
P5          15
P6          20
P7          25
P8          10
```

- Implement Merge Sort and Quick Sort in C to sort packages by weight.
- Execute both programs and record intermediate steps.
- Modify the program(s) so equal-weight packages retain their original relative order (stability), and verify using Package IDs.
- Analyse both algorithms on: duplicates, stability, number of comparisons, time complexity, and space complexity.
- Determine which algorithm is best suited when preserving original order of equal-weight packages matters.

---

## 2. Part (a): Merge Sort Implementation

Merge Sort is a divide-and-conquer algorithm. It recursively splits the array into halves, sorts each half, and merges them back using a stable merge step (the `<=` comparison in the merge function is what makes it stable).

### 2.1 Source Code — `merge_sort.c`

```c
/* ==========================================================
   MERGE SORT for Logistics Package Sorting
   Sorts packages by weight. Merge Sort is naturally STABLE:
   equal-weight packages retain their original relative order.
   Tracks number of comparisons and prints intermediate steps.
   ========================================================== */

#include <stdio.h>
#include <string.h>

#define N 8

typedef struct {
    char id[5];
    int weight;
    int original_pos;   /* used only to VERIFY stability, not for sorting */
} Package;

long comparisons = 0;

void printArray(Package arr[], int n, const char *label) {
    printf("%-28s: ", label);
    for (int i = 0; i < n; i++)
        printf("%s(%d) ", arr[i].id, arr[i].weight);
    printf("\n");
}

/* Merge two sorted halves arr[l..m] and arr[m+1..r] */
void merge(Package arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Package L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparisons++;
        /* <= keeps stability: left-half element wins ties */
        if (L[i].weight <= R[j].weight) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    printf("Merged  [%d..%d] with [%d..%d] -> ", l, m, m + 1, r);
    for (int x = l; x <= r; x++) printf("%s(%d) ", arr[x].id, arr[x].weight);
    printf("\n");
}

void mergeSort(Package arr[], int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main() {
    Package pkg[N] = {
        {"P1", 20, 0}, {"P2", 15, 1}, {"P3", 20, 2}, {"P4", 10, 3},
        {"P5", 15, 4}, {"P6", 20, 5}, {"P7", 25, 6}, {"P8", 10, 7}
    };

    printf("================ MERGE SORT ================\n");
    printArray(pkg, N, "Input (unsorted)");
    printf("---- Intermediate merge steps ----\n");

    mergeSort(pkg, 0, N - 1);

    printf("---- Final sorted output ----\n");
    printArray(pkg, N, "Sorted by weight");

    printf("\nTotal comparisons made : %ld\n", comparisons);

    printf("\nStability check (original index order preserved for equal weights):\n");
    for (int i = 0; i < N; i++)
        printf("  %s -> weight=%d, original_pos=%d\n", pkg[i].id, pkg[i].weight, pkg[i].original_pos);

    /* Automatic stability verification */
    int stable = 1;
    for (int i = 0; i < N - 1; i++) {
        if (pkg[i].weight == pkg[i + 1].weight && pkg[i].original_pos > pkg[i + 1].original_pos) {
            stable = 0;
        }
    }
    printf("\nSTABILITY RESULT: %s\n", stable ? "STABLE (original order preserved)" : "NOT STABLE");

    return 0;
}
```

### 2.2 Compilation & Execution

```
$ gcc -O2 -Wall -o merge_sort merge_sort.c
$ ./merge_sort
```

### 2.3 Program Output (actual execution)

```
================ MERGE SORT ================
Input (unsorted)            : P1(20) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P8(10)
---- Intermediate merge steps ----
Merged  [0..0] with [1..1] -> P2(15) P1(20)
Merged  [2..2] with [3..3] -> P4(10) P3(20)
Merged  [0..1] with [2..3] -> P4(10) P2(15) P1(20) P3(20)
Merged  [4..4] with [5..5] -> P5(15) P6(20)
Merged  [6..6] with [7..7] -> P8(10) P7(25)
Merged  [4..5] with [6..7] -> P8(10) P5(15) P6(20) P7(25)
Merged  [0..3] with [4..7] -> P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25)
---- Final sorted output ----
Sorted by weight            : P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25)

Total comparisons made : 16

Stability check (original index order preserved for equal weights):
  P4 -> weight=10, original_pos=3
  P8 -> weight=10, original_pos=7
  P2 -> weight=15, original_pos=1
  P5 -> weight=15, original_pos=4
  P1 -> weight=20, original_pos=0
  P3 -> weight=20, original_pos=2
  P6 -> weight=20, original_pos=5
  P7 -> weight=25, original_pos=6

STABILITY RESULT: STABLE (original order preserved)
```

**Observation:** The intermediate merge steps show pairs and sub-arrays being combined bottom-up. In the final output, equal-weight packages P4(10)/P8(10), P2(15)/P5(15), and P1(20)/P3(20)/P6(20) all appear in the **same relative order** as in the original input — confirming stability. Total comparisons recorded: **16**.

---

## 3. Part (a): Quick Sort Implementation (Standard / Unstable)

Quick Sort is also divide-and-conquer, but it partitions the array around a pivot (Lomuto scheme, last element chosen as pivot) and sorts in-place. Because elements are swapped across the array based only on the weight comparison, equal-weight elements can be reordered relative to each other.

### 3.1 Source Code — `quick_sort_unstable.c`

```c
/* ==========================================================
   QUICK SORT (STANDARD / UNSTABLE VERSION)
   Sorts packages by weight only (Lomuto partition scheme,
   last element as pivot). This version does NOT guarantee
   that equal-weight packages keep their original order.
   ========================================================== */

#include <stdio.h>

#define N 8

typedef struct {
    char id[5];
    int weight;
    int original_pos;
} Package;

long comparisons = 0;

void printArray(Package arr[], int n, const char *label) {
    printf("%-28s: ", label);
    for (int i = 0; i < n; i++)
        printf("%s(%d) ", arr[i].id, arr[i].weight);
    printf("\n");
}

void swap(Package *a, Package *b) {
    Package t = *a; *a = *b; *b = t;
}

int partition(Package arr[], int low, int high) {
    int pivot = arr[high].weight;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        comparisons++;
        if (arr[j].weight < pivot) {   /* strict '<' -> not stable */
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    printf("Pivot=%-3d partitioned [%d..%d] -> ", pivot, low, high);
    for (int x = low; x <= high; x++) printf("%s(%d) ", arr[x].id, arr[x].weight);
    printf("\n");

    return i + 1;
}

void quickSort(Package arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    Package pkg[N] = {
        {"P1", 20, 0}, {"P2", 15, 1}, {"P3", 20, 2}, {"P4", 10, 3},
        {"P5", 15, 4}, {"P6", 20, 5}, {"P7", 25, 6}, {"P8", 10, 7}
    };

    printf("============ QUICK SORT (UNSTABLE) ============\n");
    printArray(pkg, N, "Input (unsorted)");
    printf("---- Intermediate partition steps ----\n");

    quickSort(pkg, 0, N - 1);

    printf("---- Final sorted output ----\n");
    printArray(pkg, N, "Sorted by weight");

    printf("\nTotal comparisons made : %ld\n", comparisons);

    printf("\nOrder trace (original index shown for equal weights):\n");
    for (int i = 0; i < N; i++)
        printf("  %s -> weight=%d, original_pos=%d\n", pkg[i].id, pkg[i].weight, pkg[i].original_pos);

    int stable = 1;
    for (int i = 0; i < N - 1; i++) {
        if (pkg[i].weight == pkg[i + 1].weight && pkg[i].original_pos > pkg[i + 1].original_pos) {
            stable = 0;
        }
    }
    printf("\nSTABILITY RESULT: %s\n", stable ? "Order happened to be preserved" : "NOT STABLE (order changed)");

    return 0;
}
```

### 3.2 Compilation & Execution

```
$ gcc -O2 -Wall -o quick_sort_unstable quick_sort_unstable.c
$ ./quick_sort_unstable
```

### 3.3 Program Output (actual execution)

```
============ QUICK SORT (UNSTABLE) ============
Input (unsorted)            : P1(20) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P8(10)
---- Intermediate partition steps ----
Pivot=10  partitioned [0..7] -> P8(10) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P1(20)
Pivot=20  partitioned [1..7] -> P2(15) P4(10) P5(15) P1(20) P6(20) P7(25) P3(20)
Pivot=15  partitioned [1..3] -> P4(10) P5(15) P2(15)
Pivot=20  partitioned [5..7] -> P3(20) P7(25) P6(20)
Pivot=20  partitioned [6..7] -> P6(20) P7(25)
---- Final sorted output ----
Sorted by weight            : P8(10) P4(10) P5(15) P2(15) P1(20) P3(20) P6(20) P7(25)

Total comparisons made : 18

Order trace (original index shown for equal weights):
  P8 -> weight=10, original_pos=7
  P4 -> weight=10, original_pos=3
  P5 -> weight=15, original_pos=4
  P2 -> weight=15, original_pos=1
  P1 -> weight=20, original_pos=0
  P3 -> weight=20, original_pos=2
  P6 -> weight=20, original_pos=5
  P7 -> weight=25, original_pos=6

STABILITY RESULT: NOT STABLE (order changed)
```

**Observation:** The final sorted order is P8(10), P4(10), P5(15), P2(15), P1(20), P3(20), P6(20), P7(25). Compare this with the original input order: P4 appeared before P8, and P2 appeared before P5. After standard Quick Sort, P8 comes before P4 and P5 comes before P2 — the relative order of equal-weight packages has been **disturbed**. This is direct experimental proof that standard Quick Sort is **not stable**. Total comparisons recorded: **18**.

---

## 4. Part (b): Modified Quick Sort to Preserve Original Order (Stability)

To make Quick Sort behave in a stable manner, the comparison function is changed from comparing weight alone to comparing a **composite key**: `(weight, original_pos)`. `original_pos` is the package's position in the original input array (its "arrival order"). When two packages have equal weight, the one with the smaller `original_pos` is considered smaller, so ties are always broken in favour of the earlier package. This forces the output to match what a stable sort would produce, while keeping Quick Sort's in-place partitioning logic unchanged.

### 4.1 Source Code — `quick_sort_stable.c`

```c
/* ==========================================================
   QUICK SORT (MODIFIED / MADE "STABLE") - Part (b)
   Quick Sort is not naturally stable because it swaps elements
   across the array during partitioning. To make it behave in a
   stable manner, we sort using a COMPOSITE KEY:
      primary key   = weight
      secondary key = original_pos (the package's original index)
   Comparing (weight, original_pos) as a pair forces equal-weight
   packages to come out in their original relative order, which
   is exactly what a stable sort guarantees.
   ========================================================== */

#include <stdio.h>

#define N 8

typedef struct {
    char id[5];
    int weight;
    int original_pos;
} Package;

long comparisons = 0;

void printArray(Package arr[], int n, const char *label) {
    printf("%-28s: ", label);
    for (int i = 0; i < n; i++)
        printf("%s(%d) ", arr[i].id, arr[i].weight);
    printf("\n");
}

void swap(Package *a, Package *b) {
    Package t = *a; *a = *b; *b = t;
}

/* Composite comparison: weight first, original_pos breaks ties */
int isLess(Package a, Package b) {
    comparisons++;
    if (a.weight != b.weight) return a.weight < b.weight;
    return a.original_pos < b.original_pos;
}

int partition(Package arr[], int low, int high) {
    Package pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (isLess(arr[j], pivot)) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    printf("Pivot=%-3s(%d) partitioned [%d..%d] -> ", pivot.id, pivot.weight, low, high);
    for (int x = low; x <= high; x++) printf("%s(%d) ", arr[x].id, arr[x].weight);
    printf("\n");

    return i + 1;
}

void quickSort(Package arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    Package pkg[N] = {
        {"P1", 20, 0}, {"P2", 15, 1}, {"P3", 20, 2}, {"P4", 10, 3},
        {"P5", 15, 4}, {"P6", 20, 5}, {"P7", 25, 6}, {"P8", 10, 7}
    };

    printf("========== QUICK SORT (MODIFIED - STABLE) ==========\n");
    printArray(pkg, N, "Input (unsorted)");
    printf("---- Intermediate partition steps ----\n");

    quickSort(pkg, 0, N - 1);

    printf("---- Final sorted output ----\n");
    printArray(pkg, N, "Sorted by weight");

    printf("\nTotal comparisons made : %ld\n", comparisons);

    printf("\nStability check (original index order preserved for equal weights):\n");
    for (int i = 0; i < N; i++)
        printf("  %s -> weight=%d, original_pos=%d\n", pkg[i].id, pkg[i].weight, pkg[i].original_pos);

    int stable = 1;
    for (int i = 0; i < N - 1; i++) {
        if (pkg[i].weight == pkg[i + 1].weight && pkg[i].original_pos > pkg[i + 1].original_pos) {
            stable = 0;
        }
    }
    printf("\nSTABILITY RESULT: %s\n", stable ? "STABLE (original order preserved)" : "NOT STABLE");

    return 0;
}
```

### 4.2 Compilation & Execution

```
$ gcc -O2 -Wall -o quick_sort_stable quick_sort_stable.c
$ ./quick_sort_stable
```

### 4.3 Program Output (actual execution)

```
========== QUICK SORT (MODIFIED - STABLE) ==========
Input (unsorted)            : P1(20) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P8(10)
---- Intermediate partition steps ----
Pivot=P8 (10) partitioned [0..7] -> P4(10) P8(10) P3(20) P1(20) P5(15) P6(20) P7(25) P2(15)
Pivot=P2 (15) partitioned [2..7] -> P2(15) P1(20) P5(15) P6(20) P7(25) P3(20)
Pivot=P3 (20) partitioned [3..7] -> P1(20) P5(15) P3(20) P7(25) P6(20)
Pivot=P5 (15) partitioned [3..4] -> P5(15) P1(20)
Pivot=P6 (20) partitioned [6..7] -> P6(20) P7(25)
---- Final sorted output ----
Sorted by weight            : P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25)

Total comparisons made : 18

Stability check (original index order preserved for equal weights):
  P4 -> weight=10, original_pos=3
  P8 -> weight=10, original_pos=7
  P2 -> weight=15, original_pos=1
  P5 -> weight=15, original_pos=4
  P1 -> weight=20, original_pos=0
  P3 -> weight=20, original_pos=2
  P6 -> weight=20, original_pos=5
  P7 -> weight=25, original_pos=6

STABILITY RESULT: STABLE (original order preserved)
```

**Verification using Package IDs:** The final order is P4(10), P8(10), P2(15), P5(15), P1(20), P3(20), P6(20), P7(25) — identical to the Merge Sort output, and identical to the original input's relative ordering for every group of equal-weight packages (P4 before P8; P2 before P5; P1 before P3 before P6). The program's own stability check confirms: **STABLE (original order preserved)**.

---

## 5. Part (c): Comparative Analysis

### 5.1 Comparison Table

| Criterion | Merge Sort | Quick Sort (standard) | Quick Sort (modified/stable) |
|---|---|---|---|
| Duplicate values | Handled correctly; equal weights merge without disturbing order | Sorted correctly by value, but relative order of duplicates is not guaranteed | Handled correctly using (weight, original_pos) as composite key |
| Stability | Stable (proved by experiment: STABLE) | Not stable (experiment shows P8 before P4, P5 before P2 — order changed) | Made stable artificially (experiment shows STABLE) |
| No. of comparisons (n=8, this input) | 16 | 18 | 18 |
| Time complexity (Best) | O(n log n) | O(n log n) | O(n log n) |
| Time complexity (Average) | O(n log n) | O(n log n) | O(n log n) |
| Time complexity (Worst) | O(n log n) | O(n²) — sorted/reverse-sorted or many duplicates with last-element pivot | O(n²) — same worst case as standard Quick Sort |
| Space complexity | O(n) auxiliary array + O(log n) recursion stack | O(log n) avg recursion stack (in-place); O(n) worst case | O(log n) avg / O(n) worst — same as standard, in-place |
| In-place? | No (needs auxiliary arrays for merging) | Yes | Yes |

### 5.2 Discussion

- **Duplicate values:** Both algorithms correctly group equal-weight packages together in the final sorted output. The difference is only in whether the relative order **within** a duplicate group is preserved.
- **Stability:** Merge Sort is stable by design (merging always takes from the left sub-array on ties). Standard Quick Sort is not stable because partitioning swaps elements based purely on the pivot comparison, ignoring original position. The experiment above proves this concretely on the given data set.
- **Number of comparisons:** For this specific 8-element input, Merge Sort used 16 comparisons and Quick Sort used 18. In general, Merge Sort always performs close to `n·log2(n)` comparisons regardless of input arrangement. Quick Sort's comparison count depends heavily on pivot choice and input order — it can be as low as ~`n·log2(n)` or as high as ~`n(n-1)/2` in the worst case (e.g., already-sorted input with last-element pivot).
- **Time complexity:** Both have O(n log n) average and best case. Merge Sort **guarantees** O(n log n) in every case, including worst case, because it always splits exactly in half. Quick Sort degrades to O(n²) worst case when the pivot repeatedly picks the smallest or largest element (e.g., sorted input with last-element pivot, or many duplicate values clustered together).
- **Space complexity:** Merge Sort needs O(n) extra space for the temporary arrays used during merging (plus O(log n) recursion stack). Quick Sort sorts in-place, needing only O(log n) extra space on average for the recursion stack (O(n) in the worst case of unbalanced partitions).
- Making Quick Sort stable via a composite key does not change its time or space complexity class — it remains O(n log n) average / O(n²) worst, and O(log n) average extra space — it only fixes the **ordering** of ties.

---

## 6. Final Conclusion

When maintaining the original relative order of equal-weight packages is important (as in this logistics scenario, where package arrival/registration order may matter for downstream processing, auditing, or fairness), **Merge Sort is the more suitable algorithm**.

- It is naturally and unconditionally stable, with no extra engineering needed.
- It guarantees O(n log n) time complexity in **all** cases (best, average, worst) — predictable and reliable for production logistics systems.
- Its only drawback is O(n) auxiliary space, which is a reasonable trade-off for guaranteed stability and guaranteed performance.

Quick Sort **can** be made stable (as demonstrated in Part (b) using a composite `(weight, original_pos)` key), and it remains attractive when memory is tightly constrained (O(log n) average extra space) and when average-case speed with low constant factors matters more than worst-case guarantees. However, its stability then depends on the programmer explicitly adding the tie-breaking key — it is not stable "by nature" — and its worst-case time complexity remains O(n²).

**Overall recommendation:** Use **Merge Sort** as the default choice for this logistics package-sorting system, since order-preservation (stability) and predictable worst-case performance are both important operational requirements. Quick Sort (standard) should be avoided if stability matters unless explicitly modified as shown above.

---

## 7. GitHub Repository Structure

Create a repository (e.g., `logistics-package-sorting`) and upload the following structure:

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
│   └── Assignment_Report.md       (this combined document)
└── LICENSE
```

Suggested `README.md` contents: project title, problem statement, build instructions (`gcc -O2 -Wall -o <name> <file>.c`), run instructions (`./<name>`), and a short summary pointing to `docs/Assignment_Report.md` for the full analysis, comparison table, and conclusion.
