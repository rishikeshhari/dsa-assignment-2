# Package Sorting Assignment — Merge Sort vs Quick Sort (in C)

## Problem Statement
A logistics company receives package weights:
`20, 15, 20, 10, 15, 20, 25, 10` (each package has a unique package ID).

This repository implements, executes, and analyses **Merge Sort** and
**Quick Sort** in C to sort packages by weight, and additionally
implements a **stability fix** for Quick Sort so that equal-weight
packages retain their original relative (arrival) order.

## Repository Structure
```
.
├── src/
│   ├── merge_sort.c          # Standard merge sort (naturally stable)
│   ├── quick_sort.c          # Standard Lomuto-partition quick sort (NOT stable)
│   └── quick_sort_stable.c   # Modified quick sort: composite key (weight, original_index)
├── input/
│   └── input.txt             # Package IDs + weights (arrival order)
├── output/
│   ├── merge_sort_output.txt
│   ├── quick_sort_output.txt
│   └── quick_sort_stable_output.txt
├── docs/
│   ├── complexity_analysis.md
│   ├── comparison_table.md
│   └── conclusion.md
└── README.md
```

## How to Build & Run
```bash
gcc -O2 -o src/merge_sort         src/merge_sort.c
gcc -O2 -o src/quick_sort         src/quick_sort.c
gcc -O2 -o src/quick_sort_stable  src/quick_sort_stable.c

./src/merge_sort        > output/merge_sort_output.txt
./src/quick_sort        > output/quick_sort_output.txt
./src/quick_sort_stable > output/quick_sort_stable_output.txt
```
Each program prints:
- the original package order,
- step-by-step intermediate merge/partition operations,
- the final sorted order,
- the total number of comparisons performed,
- an automatic **stability check** (compares final order against each
  package's `original_index` for any equal-weight pair).

## Part (a) — Sort by weight
See `src/merge_sort.c` and `src/quick_sort.c`, and their outputs in `output/`.

## Part (b) — Preserve original order for equal weights
- `merge_sort.c` is **already stable** (uses `<=` in the merge step) — no
  modification needed. Verified: `PASSED - STABLE`.
- `quick_sort.c` (plain) is **not stable** — verified: `FAILED - NOT STABLE`
  (e.g., package P5 ends up before P2 despite arriving after it).
- `quick_sort_stable.c` is the **modified** version: it compares packages
  using the composite key `(weight, original_index)`, forcing ties to
  resolve in favour of the earlier-arrived package. Verified:
  `PASSED - STABLE (order preserved)`.

## Part (c) — Analysis & Conclusion
See:
- [`docs/complexity_analysis.md`](docs/complexity_analysis.md) — time &
  space complexity discussion for all three programs.
- [`docs/comparison_table.md`](docs/comparison_table.md) — side-by-side
  comparison table.
- [`docs/conclusion.md`](docs/conclusion.md) — final justified
  recommendation (**Merge Sort** is recommended when preserving the
  original order of equal-weight packages is important).

## How to Publish This to GitHub
```bash
cd package-sorting-assignment      # this folder
git init
git add .
git commit -m "Merge sort vs quick sort: package weight sorting assignment"
git branch -M main
git remote add origin https://github.com/<your-username>/<your-repo>.git
git push -u origin main
```
