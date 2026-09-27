/* ============================================================
   QUICK SORT on Package records (sort key = weight)
   Standard Lomuto-partition quicksort. Because sorting is done
   by SWAPPING elements across the array, equal-weight packages
   can end up in an order different from their original arrival
   order. This program is intentionally the plain / unmodified
   version so its (in)stability can be observed and compared.
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
        printf("%s(%d) ", arr[i].id, arr[i].weight);
    printf("\n");
}

void swap(Package *a, Package *b) {
    Package t = *a; *a = *b; *b = t;
}

int partition(Package arr[], int low, int high) {
    int pivot = arr[high].weight;   /* last element as pivot */
    int i = low - 1;

    printf("\nStep %d: Partitioning arr[%d..%d], pivot = %s(%d)\n",
           ++step_no, low, high, arr[high].id, pivot);

    for (int j = low; j < high; j++) {
        comparisons++;
        if (arr[j].weight <= pivot) {
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
    printf(" QUICK SORT (standard/unmodified) - Package Weight Sorting\n");
    printf("=========================================\n");
    print_array("Original Order ", pkgs, n);

    quick_sort(pkgs, 0, n - 1);

    printf("\n=========================================\n");
    printf("Final Sorted Order (by weight):\n");
    print_array("Sorted", pkgs, n);

    printf("\nTotal comparisons performed : %ld\n", comparisons);
    printf("Stability check (equal weights keep original order): %s\n",
           verify_stability(pkgs, n) ? "PASSED - STABLE" : "FAILED - NOT STABLE");

    printf("\nDetailed final list with original index (for verification):\n");
    for (int i = 0; i < n; i++)
        printf("  %s  weight=%2d  original_index=%d\n",
               pkgs[i].id, pkgs[i].weight, pkgs[i].original_index);

    return 0;
}
