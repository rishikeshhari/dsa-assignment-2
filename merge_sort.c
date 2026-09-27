/* ============================================================
   MERGE SORT on Package records (sort key = weight)
   Merge sort is naturally STABLE if, on equal keys, we always
   pick the element from the LEFT sub-array first. That is what
   the merge() function below does (condition: L[i].weight <= R[j].weight).
   ============================================================ */

#include <stdio.h>
#include <string.h>

typedef struct {
    char id[8];
    int weight;
    int original_index;   /* used only to VERIFY stability afterwards */
} Package;

long comparisons = 0;   /* global comparison counter */
int step_no = 0;

void print_array(const char *label, Package arr[], int n) {
    printf("%s: ", label);
    for (int i = 0; i < n; i++)
        printf("%s(%d) ", arr[i].id, arr[i].weight);
    printf("\n");
}

void merge(Package arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Package L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    printf("\nStep %d: Merging  ", ++step_no);
    print_array("Left", L, n1);
    printf("           with     ");
    print_array("Right", R, n2);

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparisons++;
        /* <=  keeps the LEFT element first on tie => STABLE */
        if (L[i].weight <= R[j].weight) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    printf("           Result -> ");
    print_array("Merged", arr + l, r - l + 1);
}

void merge_sort(Package arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int verify_stability(Package arr[], int n) {
    /* For every pair of equal-weight packages, original relative
       order must be preserved (original_index increasing) */
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
    printf(" MERGE SORT - Package Weight Sorting\n");
    printf("=========================================\n");
    print_array("Original Order ", pkgs, n);

    merge_sort(pkgs, 0, n - 1);

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
