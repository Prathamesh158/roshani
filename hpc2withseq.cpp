#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <omp.h>

using namespace std;

// Function to perform parallel bubble sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    #pragma omp parallel
    {
        do {
            swapped = false;
            #pragma omp for
            for (int i = 0; i < n - 1; ++i) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
        } while (swapped);
    }
}

// Function to perform merge operation for merge sort
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Function to perform parallel merge sort
void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

// Function to measure execution time of a function
template<typename Func, typename... Args>
double measureExecutionTime(Func func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    // Input elements from user
    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    // Sequential Bubble Sort
    vector<int> arr_seq = arr;
    double seq_bubble_time = measureExecutionTime(sort<vector<int>::iterator>, arr_seq.begin(), arr_seq.end());
    cout << "Sequential Bubble Sort: ";
    for (const auto& num : arr_seq)
        cout << num << " ";
    cout << endl;
cout<<"Execution time: "<<seq_bubble_time<<endl;
    // Parallel Bubble Sort
    vector<int> arr_par = arr;
    double par_bubble_time = measureExecutionTime(parallelBubbleSort, arr_par);
    cout << "Parallel Bubble Sort: ";
    for (const auto& num : arr_par)
        cout << num << " ";
    cout << endl;
cout<<"Execution time: "<<par_bubble_time<<endl;
    // Sequential Merge Sort
    vector<int> arr_seq_merge = arr;
    double seq_merge_time = measureExecutionTime(sort<vector<int>::iterator>, arr_seq_merge.begin(), arr_seq_merge.end());
    cout << "Sequential Merge Sort: ";
    for (const auto& num : arr_seq_merge)
        cout << num << " ";
    cout << endl;
cout<<"Execution time: "<<seq_merge_time<<endl;

    // Parallel Merge Sort
    vector<int> arr_par_merge = arr;
    double par_merge_time = measureExecutionTime(parallelMergeSort, arr_par_merge, 0, arr_par_merge.size() - 1);
    cout << "Parallel Merge Sort: ";
    for (const auto& num : arr_par_merge)
        cout << num << " ";
    cout << endl;
    cout<<"Execution time: "<<par_merge_time<<endl;

    return 0;
}
