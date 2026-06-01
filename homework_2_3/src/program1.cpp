#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// 驗證陣列是否正確排序（由小到大）
bool IsSorted(const int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) return false;
    }
    return true;
}

// 陣列複製函數
void CopyArray(const int *src, int *dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

// 產生 Insertion Sort 最壞情況資料（逆序）
void GenerateInsertionWorst(int *a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
}

// 隨機排列洗牌產生器（遵循 Fisher-Yates 演算法精神修正 0-based index）
void Permute(int *a, int n) {
    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % (i + 1);
        swap(a[j], a[i]);
    }
}

// Merge Sort 最壞情況逆向推導核心
void MergeWorstBackward(int *a, int *temp, int left, int right) {
    if (left >= right) return;
    if (right - left == 1) {
        swap(a[left], a[right]);
        return;
    }

    int mid = left + (right - left) / 2;
    int t1 = left, t2 = mid + 1;
    
    for (int i = left; i <= right; i++) {
        temp[i] = a[i];
    }
    
    for (int i = left; i <= right; i += 2) {
        a[t1++] = temp[i];
        if (i + 1 <= right) {
            a[t2++] = temp[i + 1];
        }
    }
    
    MergeWorstBackward(a, temp, left, mid);
    MergeWorstBackward(a, temp, mid + 1, right);
}

// 產生 Merge Sort 最壞情況資料
void GenerateMergeWorst(int *a, int n) {
    for (int i = 0; i < n; i++) a[i] = i + 1;
    int *temp = new int[n];
    MergeWorstBackward(a, temp, 0, n - 1);
    delete[] temp;
}

// 1. Insertion Sort 實作
void InsertionSort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// Quick Sort 三數取中法選 Pivot
int MedianOfThree(int *a, int low, int high) {
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) swap(a[low], a[mid]);
    if (a[low] > a[high]) swap(a[low], a[high]);
    if (a[mid] > a[high]) swap(a[mid], a[high]);
    return mid; // 回傳中間值的索引
}

// Quick Sort 遞迴核心（採用 Lomuto 劃分架構）
void QuickSortRecursive(int *a, int low, int high) {
    if (low < high) {
        int pivotIdx = MedianOfThree(a, low, high);
        int pivot = a[pivotIdx];
        swap(a[pivotIdx], a[high]); // 將 Pivot 藏至尾端
        
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (a[j] <= pivot) {
                i++;
                swap(a[i], a[j]);
            }
        }
        swap(a[i + 1], a[high]); // 把 Pivot 放回正確分割點
        int p = i + 1;

        QuickSortRecursive(a, low, p - 1);
        QuickSortRecursive(a, p + 1, high);
    }
}

// 2. Quick Sort 外部調用介面
void QuickSort(int *a, int n) {
    QuickSortRecursive(a, 0, n - 1);
}

// Merge Sort 合併雙子陣列（內部優化版：不重複 new 空間）
void Merge(int *a, int left_start, int mid, int right_end, int *temp) {
    int i = left_start;
    int j = mid + 1;
    int k = left_start;

    while (i <= mid && j <= right_end) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= right_end) temp[k++] = a[j++];

    for (i = left_start; i <= right_end; i++) {
        a[i] = temp[i];
    }
}

// 3. Iterative Merge Sort 實作 (已移除內層迴圈 new/delete 以優化效能)
void IterativeMergeSort(int *a, int n) {
    if (n < 2) return;
    int *temp = new int[n]; // 在最外層配置單一輔助空間
    for (int curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            int mid = min(left_start + curr_size - 1, n - 1);
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            Merge(a, left_start, mid, right_end, temp);
        }
    }
    delete[] temp;
}

// Heap Sort 的 MaxHeap 下沉調整函數
void Heapify(int *a, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        Heapify(a, n, largest);
    }
}

// 4. Heap Sort 實作 (由小到大排序需採用 MaxHeap)
void HeapSort(int *a, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) Heapify(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        Heapify(a, i, 0);
    }
}

// 複合排序之黃金交叉點閾值
const int CROSSOVER_N = 25; 

// 5. Composite Sort 實作
void CompositeSort(int *a, int n) {
    if (n <= CROSSOVER_N) {
        // 小規模數據採用常數開銷極低的插入排序
        void InsertionSort(int *a, int n);
        InsertionSort(a, n);
    } else {
        // 大規模數據採用最壞情況下最穩定的堆積排序
        void HeapSort(int *a, int n);
        HeapSort(a, n);
    }
}

// 實驗計時器框架（處理小測資重複曝光優化）
double TimeExperiment(void (*sortFunc)(int*, int), int *data, int n, int min_repeats = 1) {
    int *test_array = new int[n];
    int repeats = min_repeats;
    
    if (n < 500) repeats = 5000;
    else if (n < 2000) repeats = 500;

    auto start = chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        CopyArray(data, test_array, n);
        sortFunc(test_array, n);
    }
    auto end = chrono::high_resolution_clock::now();
    
    if (!IsSorted(test_array, n)) {
        cout << "\n[ERROR] Sorting Failed!" << endl;
    }

    delete[] test_array;
    chrono::duration<double, nano> elapsed = end - start;
    return (elapsed.count() / repeats) / 1000000.0; // 轉換為毫秒 (ms)
}

int main() {
    srand(2026); // 設定固定隨機種子以利實驗重現
    
    auto t_res = chrono::high_resolution_clock::period::num / (double)chrono::high_resolution_clock::period::den;
    cout << "計時器精度基準 Delta: " << t_res * 1e9 << " ns\n\n";

    int target_n[] = {20, 50, 100, 500, 1000, 2000, 3000, 4000, 5000};
    int num_sizes = sizeof(target_n) / sizeof(target_n[0]);
    const int RANDOM_PERMUTATIONS = 15;

    // ================= 最壞情況實驗流程 =================
    cout << "=== 1. Worst-Case 實測數據 (單位: ms) ===" << endl;
    cout << "n,Insertion,Quick(M3),Merge(Iter),Heap,Composite" << endl;

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = target_n[idx];
        int *ins_worst = new int[n];
        int *mrg_worst = new int[n];
        int *rand_data = new int[n];

        GenerateInsertionWorst(ins_worst, n);
        GenerateMergeWorst(mrg_worst, n);

        double t_ins = TimeExperiment(InsertionSort, ins_worst, n);
        double t_mrg = TimeExperiment(IterativeMergeSort, mrg_worst, n);

        double t_qck_max = 0.0;
        double t_hea_max = 0.0;
        
        // 隨機洗牌 15 次，抓取最大值作為最壞情況近似值
        for (int p = 0; p < RANDOM_PERMUTATIONS; p++) {
            for (int i = 0; i < n; i++) rand_data[i] = i + 1;
            Permute(rand_data, n);
            
            double t_qck_curr = TimeExperiment(QuickSort, rand_data, n);
            double t_hea_curr = TimeExperiment(HeapSort, rand_data, n);
            
            if (t_qck_curr > t_qck_max) t_qck_max = t_qck_curr;
            if (t_hea_curr > t_hea_max) t_hea_max = t_hea_curr;
        }

        double t_comp = TimeExperiment(CompositeSort, rand_data, n);

        cout << n << "," 
             << fixed << setprecision(5) << t_ins << ","
             << t_qck_max << ","
             << t_mrg << ","
             << t_hea_max << ","
             << t_comp << endl;

        delete[] ins_worst;
        delete[] mrg_worst;
        delete[] rand_data;
    }

    // ================= 平均情況實驗流程 =================
    cout << "\n=== 2. Average-Case 實測數據 (單位: ms) ===" << endl;
    cout << "n,Insertion_Avg,Quick_Avg,Merge_Avg,Heap_Avg" << endl;

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = target_n[idx];
        int *rand_data = new int[n];
        int *test_array = new int[n];
        
        double total_ins = 0, total_qck = 0, total_mrg = 0, total_hea = 0;
        const int AVG_TRIALS = 30; // 遵循題目規範，採用 30 組獨立不同的隨機排列

        for (int t = 0; t < AVG_TRIALS; t++) {
            for (int i = 0; i < n; i++) rand_data[i] = i + 1;
            Permute(rand_data, n);

            auto s1 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); InsertionSort(test_array, n);
            auto e1 = chrono::high_resolution_clock::now();
            total_ins += chrono::duration<double, milli>(e1 - s1).count();

            auto s2 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); QuickSort(test_array, n);
            auto e2 = chrono::high_resolution_clock::now();
            total_qck += chrono::duration<double, milli>(e2 - s2).count();

            auto s3 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); IterativeMergeSort(test_array, n);
            auto e3 = chrono::high_resolution_clock::now();
            total_mrg += chrono::duration<double, milli>(e3 - s3).count();

            auto s4 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); HeapSort(test_array, n);
            auto e4 = chrono::high_resolution_clock::now();
            total_hea += chrono::duration<double, milli>(e4 - s4).count();
        }

        cout << n << "," 
             << fixed << setprecision(5) << (total_ins / AVG_TRIALS) << ","
             << (total_qck / AVG_TRIALS) << ","
             << (total_mrg / AVG_TRIALS) << ","
             << (total_hea / AVG_TRIALS) << endl;

        delete[] rand_data;
        delete[] test_array;
    }

    return 0;
}