#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

// LSD Radix Sort
void lsdRadixSort(vector<int>& arr) {
    // Find maximum value to know number of digits
    int maxVal = *max_element(arr.begin(), arr.end());
    int exp = 1;
    // Output array
    vector<int> output(arr.size());
    // Counting sort for each digit
    while (maxVal / exp > 0) {
        vector<int> count(10, 0);
        // Count the number of occurrences of each digit
        for (int i = 0; i < arr.size(); i++) {
            count[(arr[i] / exp) % 10]++;
        }
        // Calculate the cumulative count
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }
        // Build the output array
        for (int i = arr.size() - 1; i >= 0; i--) {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }
        // Copy the output array to the original array
        for (int i = 0; i < arr.size(); i++) {
            arr[i] = output[i];
        }
        exp *= 10;
    }
}

// Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Wrapper for mergeSort to match the expected function signature
void mergeSortWrapper(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

// Generate array with fixed digit count
vector<int> generateFixedDigitArray(int size, int m) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    int lower = pow(10, m - 1);
    int upper = pow(10, m) - 1;
    uniform_int_distribution<> dis(lower, upper);
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Generate array with large digit range
vector<int> generateLargeRangeArray(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000000);
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Measure sorting time
double measureSortTime(void (*sortFunc)(vector<int>&), vector<int> arr, int repeat) {
    double totalTime = 0;
    for (int i = 0; i < repeat; i++) {
        vector<int> tempArr = arr;
        auto start = chrono::high_resolution_clock::now();
        sortFunc(tempArr);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        totalTime += duration.count();
    }
    return totalTime / repeat;
}

int main() {
    int DotsNum = 100; // Number of values to generate
    int MinNumValue = 10; // Minimum value for size
    int MaxNumValue = 100000; // Maximum value for size

    int MinDigits = 1; // Minimum number of digits for non-fixed digit array
    int MaxDigits = 9; // Maximum number of digits for non-fixed digit array
    int FixedDigits = 5; // Number of digits for fixed digit array

    int repeat = 10; // Number of repetitions for each size and taking average

    // Generate logspace sizes
    vector<int> sizes(DotsNum);
    for (int i = 0; i < DotsNum; i++) {
        sizes[i] = static_cast<int>(pow(10, log10(MinNumValue) + i * (log10(MaxNumValue) - log10(MinNumValue)) / (DotsNum - 1)));
    }
    ofstream outFile("build/results.csv");
    outFile << "Size,LSD Fixed,Merge Fixed,LSD Range,Merge Range\n";
    for (int size : sizes) {
        vector<int> fixedDigitArray = generateFixedDigitArray(size, FixedDigits);
        vector<int> largeRangeArray = generateLargeRangeArray(size);

        double lsdFixedTime = measureSortTime(lsdRadixSort, fixedDigitArray, repeat);
        double mergeFixedTime = measureSortTime(mergeSortWrapper, fixedDigitArray, repeat);
        double lsdRangeTime = measureSortTime(lsdRadixSort, largeRangeArray, repeat);
        double mergeRangeTime = measureSortTime(mergeSortWrapper, largeRangeArray, repeat);

        outFile << size << "," << lsdFixedTime << "," << mergeFixedTime << "," << lsdRangeTime << "," << mergeRangeTime << "\n";
    }
    outFile.close();

    return 0;
}