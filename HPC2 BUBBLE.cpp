#include <iostream>
#include <algorithm>
#include <omp.h>

// Function to perform Bubble Sort
void bubbleSort(int arr[], int n) {
    #pragma omp parallel for
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform Merge Sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        std::inplace_merge(arr + left, arr + mid + 1, arr + right + 1);
    }
}

int main() {
    int size;
    std::cout << "Enter the size of the array: ";
    std::cin >> size;

    int arr[size];
    std::cout << "Enter the elements of the array:\n";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    double startTime, endTime;

    std::cout << "Choose sorting algorithm:\n";
    std::cout << "1. Parallel Bubble Sort\n";
    std::cout << "2. Parallel Merge Sort\n";
    std::cout << "Enter your choice (1 or 2): ";
    
    int choice;
    std::cin >> choice;

    startTime = omp_get_wtime(); // Start timer
    switch (choice) {
        case 1:
            bubbleSort(arr, size);
            break;
        case 2:
            mergeSort(arr, 0, size - 1);
            break;
        default:
            std::cout << "Invalid choice\n";
            return 1;
    }
    endTime = omp_get_wtime(); // End timer

    std::cout << "Sorted array:\n";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Time taken: " << endTime - startTime << " seconds" << std::endl;

    return 0;
}
