/* ============================================================
   QUICK SORT - MODIFIED TO PRESERVE ORIGINAL RELATIVE ORDER
   Quick sort's swap-based partitioning is not inherently stable.
   To make equal-weight packages retain their original relative
   order, we sort by a COMPOSITE KEY: (weight, original_index).
   Since every package has a distinct original_index, ties on
   weight are always broken in favour of the package that
   arrived earlier -> the final output reproduces the original
   relative order for equal weights, verified using package IDs.
   ============================================================ */

#include <stdio.h>
#include <string.h>

typedef struct {
    char id[8];
    int weight;
    int original_index;
} Package;

long comparisons = 0;
int step_no = 0;

void print_array(const char *label, Package arr[], int n) {
    printf("%s: ", label);
    for (int i = 0; i < n; i++)
        printf("%s(w=%d,idx=%d) ", arr[i].id, arr[i].weight, arr[i].original_index);
    printf("\n");
}

void swap(Package *a, Package *b) {
    Package t = *a; *a = *b; *b = t;
}

/* returns 1 if a should come before or equal to b */
int less_or_equal(Package a, Package b) {
    comparisons++;
    if (a.weight != b.weight) return a.weight < b.weight;
    return a.original_index < b.original_index;  /* tie-breaker */
}

int partition(Package arr[], int low, int high) {
    Package pivot = arr[high];
    int i = low - 1;

    printf("\nStep %d: Partitioning arr[%d..%d], pivot = %s(w=%d,idx=%d)\n",
           ++step_no, low, high, pivot.id, pivot.weight, pivot.original_index);

    for (int j = low; j < high; j++) {
        if (less_or_equal(arr[j], pivot)) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);

    print_array("           Result -> ", arr + low, high - low + 1);
    return i + 1;
}

void quick_sort(Package arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int verify_stability(Package arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].weight == arr[j].weight &&
                arr[i].original_index > arr[j].original_index)
                return 0;
        }
    }
    return 1;
}

int main() {
    Package pkgs[] = {
        {"P1", 20, 0},
        {"P2", 15, 1},
        {"P3", 20, 2},
        {"P4", 10, 3},
        {"P5", 15, 4},
        {"P6", 20, 5},
        {"P7", 25, 6},
        {"P8", 10, 7}
    };
    int n = sizeof(pkgs) / sizeof(pkgs[0]);

    printf("=========================================\n");
    printf(" QUICK SORT (MODIFIED, stable-order preserved) - Package Weight Sorting\n");
    printf("=========================================\n");
    print_array("Original Order ", pkgs, n);

    quick_sort(pkgs, 0, n - 1);

    printf("\n=========================================\n");
    printf("Final Sorted Order (by weight, ties broken by arrival order):\n");
    print_array("Sorted", pkgs, n);

    printf("\nTotal comparisons performed : %ld\n", comparisons);
    printf("Stability check (equal weights keep original order): %s\n",
           verify_stability(pkgs, n) ? "PASSED - STABLE (order preserved)" : "FAILED - NOT STABLE");

    printf("\nDetailed final list with original index (for verification):\n");
    for (int i = 0; i < n; i++)
        printf("  %s  weight=%2d  original_index=%d\n",
               pkgs[i].id, pkgs[i].weight, pkgs[i].original_index);

    return 0;
}
