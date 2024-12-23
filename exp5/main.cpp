#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 起泡排序 (Bubble Sort)
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// 插入排序 (Insertion Sort)
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 选择排序 (Selection Sort)
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        int minIdx = i;
        for (int j = i+1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        swap(arr[i], arr[minIdx]);
    }
}

// 归并排序 (Merge Sort)
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
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

// 快速排序 (Quick Sort)
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 堆排序 (Heap Sort)
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 包装函数，用于符合 void (*)(std::vector<int>&) 类型
void mergeSortWrapper(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

void quickSortWrapper(vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

// 测试排序算法
void testSortingAlgorithm(void (*sortFunc)(vector<int>&), const string& algoName, const vector<int>& data) {
    vector<int> arr = data;
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << algoName << " took " << duration.count() << " microseconds." << endl;
}

void generateSequence(vector<int>& arr, const string& type) {
    int n = arr.size();
    if (type == "ascending") {
        for (int i = 0; i < n; ++i) {
            arr[i] = i;
        }
    } else if (type == "descending") {
        for (int i = 0; i < n; ++i) {
            arr[i] = n - i - 1;
        }
    } else { // random
        for (int i = 0; i < n; ++i) {
            arr[i] = rand() % n;
        }
    }
}

int main() {
    const int n = 10000; // 测试数据规模
    vector<int> arr(n);

    // 测试顺序序列
    cout << "Testing with ascending order sequence:" << endl;
    generateSequence(arr, "ascending");
    testSortingAlgorithm(bubbleSort, "Bubble Sort", arr);
    testSortingAlgorithm(insertionSort, "Insertion Sort", arr);
    testSortingAlgorithm(selectionSort, "Selection Sort", arr);
    testSortingAlgorithm(mergeSortWrapper, "Merge Sort", arr);
    testSortingAlgorithm(quickSortWrapper, "Quick Sort", arr);
    testSortingAlgorithm(heapSort, "Heap Sort", arr);

    // 测试逆序序列
    cout << "\nTesting with descending order sequence:" << endl;
    generateSequence(arr, "descending");
    testSortingAlgorithm(bubbleSort, "Bubble Sort", arr);
    testSortingAlgorithm(insertionSort, "Insertion Sort", arr);
    testSortingAlgorithm(selectionSort, "Selection Sort", arr);
    testSortingAlgorithm(mergeSortWrapper, "Merge Sort", arr);
    testSortingAlgorithm(quickSortWrapper, "Quick Sort", arr);
    testSortingAlgorithm(heapSort, "Heap Sort", arr);

    // 测试随机序列
    cout << "\nTesting with random order sequence:" << endl;
    generateSequence(arr, "random");
    testSortingAlgorithm(bubbleSort, "Bubble Sort", arr);
    testSortingAlgorithm(insertionSort, "Insertion Sort", arr);
    testSortingAlgorithm(selectionSort, "Selection Sort", arr);
    testSortingAlgorithm(mergeSortWrapper, "Merge Sort", arr);
    testSortingAlgorithm(quickSortWrapper, "Quick Sort", arr);
    testSortingAlgorithm(heapSort, "Heap Sort", arr);

    return 0;
}
