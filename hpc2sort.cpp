#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;

void parallelBubbleSort(vector<int>& arr) {
    bool swapped;
    do {
        swapped = false;
        #pragma omp parallel for
        for (int i = 0; i < arr.size() - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);
}

void parallelMergeSort(vector<int>& arr) {
    if (arr.size() <= 1) 
    return;
    int mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());
    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(left);
        #pragma omp section
        parallelMergeSort(right);
    }
    merge(left.begin(), left.end(), right.begin(), right.end(), arr.begin());
}

template<typename Func, typename... Args>
double measureExecutionTime(Func func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...);
    return chrono::duration<double>(chrono::high_resolution_clock::now() - start).count();
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];
    auto display = [](const string& label, const vector<int>& arr) {
        cout << label << ": ";
        for (const auto& num : arr)
            cout << num << " ";
        cout << endl;
    };
    display("Original array", arr);
    vector<int> arr_bubble = arr, arr_merge = arr;
    double bubble_time = measureExecutionTime(parallelBubbleSort, arr_bubble);
    display("Sorted array (Parallel Bubble Sort)", arr_bubble);
    cout << "Bubble Sort Execution time: " << bubble_time << " seconds" << endl;
    double merge_time = measureExecutionTime(parallelMergeSort, arr_merge);
    display("Sorted array (Parallel Merge Sort)", arr_merge);
    cout << "Merge Sort Execution time: " << merge_time << " seconds" << endl;
return 0;
}