/*
* АиСД-2, 2021, задание 5
* Точилина Полина Витальевна, БПИ199
* Visual Studio 2019
* Сделано всё (вроде)
*/

#include <iostream>
#include <random>
#include <string> 
#include <chrono>
#include <fstream>

using std::swap;
using std::string;

// --- ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ СОРТИРОВОК ----

// Нахождение обычного минимума (для сортировки подсчетом)
int myMin(int* numbers, int arr_size) {
    int res = numbers[0];

    for (int i = 1; i < arr_size; i++)
        if (numbers[i] < res)
            res = numbers[i];

    return res;
}

// Нахождение обычного максимума (для сортировки подсчетом)
int myMax(int* numbers, int arr_size) {
    int res = numbers[0];

    for (int i = 1; i < arr_size; i++)
        if (numbers[i] > res)
            res = numbers[i];

    return res;
}

// Копирование одного массива в другой (для линейных сортировок)
void copyTo(int* from, int* to, int n) {
    for (int i = 0; i < n; i++)
        to[i] = from[i];
}

// Нахождение минимума по основанию 256 (для цифровой сортировки)
int min256(int* numbers, int arr_size, int exp) {
    int res = (numbers[0] >> 8 * exp) & 255;

    for (int i = 1; i < arr_size; i++)
        if (((numbers[i] >> 8 * exp) & 255) < res)
            res = (numbers[i] >> 8 * exp) & 255;

    return res;
}

// Нахождение максимума по основанию 256 (для цифровой сортировки)
int max256(int* numbers, int arr_size, int exp) {
    int res = (numbers[0] >> 8 * exp) & 255;

    for (int i = 1; i < arr_size; i++)
        if (((numbers[i] >> 8 * exp) & 255) > res)
            res = (numbers[i] >> 8 * exp) & 255;

    return res;
}

// ------------------ ИТЕРАЦИОННЫЕ --------------

/// <summary>
/// Сортировка выбором. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив.</param>
/// <param name="n"> Длина сортируемого массива. </param>
void selectionSort(int* arr, int n) {
	int minIndex;
	for (auto i = 0; i < n - 1; i++) {
		minIndex = i;
		for (auto j = i + 1; j < n; j++)
			if (arr[j] < arr[minIndex])
				minIndex = j;
		if (arr[minIndex] != arr[i])
			swap(arr[i], arr[minIndex]);
	}
}

/// <summary>
/// Сортировка пузырьком. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void bubbleSort(int* arr, int n) {
	for (auto i = 0; i < n - 1; i++)
		for (auto j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
}

/// <summary>
/// Сортировка простыми вставками. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void insertionSort(int* arr, int n) {
	int value, j;

	for (auto i = 1; i < n; i++) {
		value = arr[i];
		j = i - 1;

        while (j >= 0 && arr[j] > value) {
            arr[j + 1] = arr[j];
            j--;
        }

		arr[j + 1] = value;
	}
}

// ------------------ ЛИНЕЙНЫЕ ------------------

/// <summary>
/// Сортировка подсчетом (устойчивая). Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void countingSort(int* arr, int n) {
    // находим минимум и максимум
    int min = myMin(arr, n);
    int max = myMax(arr, n);

    // массив для подсчета
    int* couArr = new int[max - min + 1];
    for (auto i = 0; i < max - min + 1; i++)
        couArr[i] = 0;

    // массив для записи результата
    int* resArr = new int[n];

    // считаем элементы
    for (auto i = 0; i < n; i++)
        couArr[arr[i] - min]++;

    // сумма
    for (auto i = 1; i < max - min + 1; i++)
        couArr[i] += couArr[i - 1];

    // записываем результирующий массив с конца, чтобы получить стабильную сортировку
    for (auto i = n - 1; i >= 0; i--) {

        resArr[couArr[arr[i] - min] - 1] = arr[i];
        couArr[arr[i] - min]--;
    }

    copyTo(resArr, arr, n);

    // освобождаем память
    delete[] resArr;
    delete[] couArr;
}

/// <summary>
/// Сортировка подсчетом (по основанию 256) для цифровой сортировки. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void countingSortForRadix(int* arr, int n, int exp) {
    // находим минимум и максимум
    int min = min256(arr, n, exp);
    int max = max256(arr, n, exp);

    // массив для подсчета
    int* couArr = new int[max - min + 1];
    for (auto i = 0; i < max - min + 1; i++)
        couArr[i] = 0;

    // массив для записи результата
    int* resArr = new int[n];

    // считаем элементы
    for (auto i = 0; i < n; i++)
        couArr[((arr[i] >> 8 * exp) & 255) - min]++;
    
    // сумма
    for (auto i = 1; i < max - min + 1; i++)
        couArr[i] += couArr[i - 1];

    // записываем результирующий массив с конца, чтобы получить стабильную сортировку
    for (auto i = n - 1; i >= 0; i--) {

        resArr[couArr[((arr[i] >> 8 * exp) & 255) - min] - 1] = arr[i];
        couArr[((arr[i] >> 8 * exp) & 255) - min]--;
    }

    copyTo(resArr, arr, n);

    // освобождаем память
    delete[] resArr;
    delete[] couArr;
}

/// <summary>
/// Цифровая сортировка (по основанию 256). Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void radixSort(int* arr, int n) {
    int max = arr[0];

    // Находим максимум
    for (auto i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];

    // Стабильная сортировка для каждой цифры (по основанию 256)
    for (auto exp = 0; max >> 8 * exp > 0; exp++)
        countingSortForRadix(arr, n, exp);
}

// ------------------ РЕКУРСИВНЫЕ ---------------

/// <summary>
/// Быстрая сортировка (средний опороный). Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void recursiveQuickSort(int* arr, int l, int r) {
    int m = l + ((r - l) >> 1);
    int pivot = arr[m];
    int i = l;
    int j = r;

    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;

        while (arr[j] > pivot)
            j--;

        if (i <= j) swap(arr[i++], arr[j--]);
    }

    // рекурсия в левой части
    if (j > l)
        recursiveQuickSort(arr, l, j);

    // рекурсия в правой части
    if (i < r)
        recursiveQuickSort(arr, i, r);
}

/// <summary>
/// Обертка для быстрой сортировки (средний опороный). Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void quickSort(int* arr, int n) {
    recursiveQuickSort(arr, 0, n - 1);
}

void heapify(int* arr, int array_size, int i) {
    int largest = i;
    // индекс левого ребенка
    int l = 2 * i + 1;
    // индекс правого ребенка
    int r = 2 * i + 2;

    // если левый больше
    if (l < array_size && arr[l] > arr[largest])
        largest = l;

    // если правый больше
    if (r < array_size && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        if (largest * 2 + 1 < array_size)
            heapify(arr, array_size, largest);
    }
}

void buildHeap(int* arr, int array_size) {
    for (auto i = (array_size / 2) - 1; i > -1; i--) {
        heapify(arr, array_size, i);
    }
}

/// <summary>
/// Пирамидальная сортировка. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void heapSort(int* arr, int n)
{
    buildHeap(arr, n);

    // достаем корневые элементы кучи в начало массива
    for (auto i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        // передаем уменьшенный размер кучи
        heapify(arr, i, 0);
    }
}

// Мержит два подмассива
// Первый подмассив arr[l..m]
// Второй подмассив arr[m+1..r]
void merge(int* arr, int l, int m, int r)
{
    int len1 = m - l + 1;
    int len2 = r - m;

    int* left = new int[len1];
    int* right = new int[len2];

    // Копируем элементы во временные массивы
    for (auto i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (auto j = 0; j < len2; j++)
        right[j] = arr[m + 1 + j];

    // Мержим два временных массива в изначальный

    int i = 0, j = 0, k = l;

    while (i < len1 && j < len2) {
        if (left[i] <= right[j])
            arr[k] = left[i++];
        else
            arr[k] = right[j++];
        k++;
    }

    // Копируем оставшиеся элементы из L
    while (i < len1)
        arr[k++] = left[i++];

    // Копируем оставшиеся элементы из R
    while (j < len2)
        arr[k++] = right[j++];

    delete[] left;
    delete[] right;
}

/// <summary>
/// Сортировка слиянием. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void recursiveMergeSort(int* arr, int l, int r) {
    if (l >= r) return;

    // находим средний
    int m = l + (r - l) / 2;
    // рекурсия в левом подмассиве
    recursiveMergeSort(arr, l, m);
    // рекурсия в правом подмассиве
    recursiveMergeSort(arr, m + 1, r);
    // мерджим два подмассива
    merge(arr, l, m, r);
}

/// <summary>
/// Обертка для сортировки слиянием. Ответом является переданный массив.
/// </summary>
/// <param name="arr"> Сортируемый массив. </param>
/// <param name="n"> Длина сортируемого массива. </param>
void mergeSort(int* arr, int n) {
    recursiveMergeSort(arr, 0, n - 1);
}

// ------------------ РЕШЕНИЕ --------------------
typedef void(*FunctionPointer)(int*, int);

class Settings {
public:
    // названия сортировок
    static string* functionNames;
    // указатели на сортировки
    static FunctionPointer* functionPointers;

    // количество сортировок
    static const int sortAmount = 8;
    // размер эталонного массива
    static const int standartArraySize = 4100;

    static int* random0To5;
    static int* random0To4000;
    static int* almostSorted;
    static int* reverseSorted;

    // типы массивов
    static string* arrayTypes;
    // массив эталонных массивов каждого типа
    static int** standartArrays;

    static void configure() {
        srand(time(NULL));

        random0To5 = new int[standartArraySize];
        random0To4000 = new int[standartArraySize];
        almostSorted = new int[standartArraySize];
        reverseSorted = new int[standartArraySize];

        // заполняем эталонные массивы
        for (auto i = 0; i < standartArraySize; i++) {
            random0To5[i] = rand() % 6;
            random0To4000[i] = rand() % 4001;
            almostSorted[i] = i;
            reverseSorted[i] = standartArraySize - i;
        }

        // слегка перемешиваем почти сортированный массив
        for (auto i = 0; i < standartArraySize; i += 15)
            if(i + 1 < standartArraySize) swap(almostSorted[i], almostSorted[i + 1]);

        // заолняем массив массивов
        standartArrays = new int* [4];
        standartArrays[0] = random0To5;
        standartArrays[1] = random0To4000;
        standartArrays[2] = almostSorted;
        standartArrays[3] = reverseSorted;
    }

    static void free(){
        delete[] functionPointers;
        delete[] functionNames;
        delete[] arrayTypes;
        for (auto i = 0; i < 4; i++)
            delete[] standartArrays[i];
        delete[] standartArrays;
    }
};

string* Settings::functionNames;
FunctionPointer* Settings::functionPointers;

int* Settings::random0To5;
int* Settings::random0To4000;
int* Settings::almostSorted;
int* Settings::reverseSorted;

string* Settings::arrayTypes;
int** Settings::standartArrays;

bool isSorted(int* arr, int n) {
    for (auto i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return false;
    return true;
}

string countForLength(int length) {
    int* tmp = new int[length];

    string res = std::to_string(length);

    // Для каждой сортировки
    for (auto sortType = 0; sortType < Settings::sortAmount; sortType++) {
        // Для каждого типа массива
        for (auto arrayType = 0; arrayType < 4; arrayType++) {
            long long sumTime = 0;
            // 50 запусков и 3 холостых
            for (auto i = 0; i < 53; i++) {
                // Создаем массив для сортировки
                copyTo(Settings::standartArrays[arrayType], tmp, length);

                auto startTime = std::chrono::system_clock::now();
                // Сортируем
                (*(Settings::functionPointers)[sortType])(tmp, length);
                auto endTime = std::chrono::system_clock::now();

                // записываем время
                if (i > 2)
                    sumTime += (std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime)).count();
            }
            // получаем среднее
            sumTime /= 50;
            res += ";" + std::to_string(sumTime);

            if (!isSorted(tmp, length)) 
                std::cout << "Массив неверно отсортирован, индекс сортировки: " << sortType << "; индекс типа массива: " << arrayType;
        }
    }

    delete[] tmp;
    return res;
}

void writeToFile(string path, string data) {
    std::ofstream outp(path);
    outp << data;
    outp.close();
}

int main()
{
    Settings::functionNames = new string[]{
        "Bubble", "Selection", "Insertion", "Counting", 
        "Radix", "Merge", "Quick", "Heap",
    };

    Settings::functionPointers = new FunctionPointer[] {
        bubbleSort, selectionSort, insertionSort, countingSort, 
        radixSort, mergeSort, quickSort, heapSort,
    };

    Settings::arrayTypes = new string[]{
        "random [0, 5]", "random [0, 4000]", "almost sorted", "reverse sorted",
    };

    Settings::configure();

    string headerStr = "Size";
    for (auto i = 0; i < Settings::sortAmount; i++)
        for (auto j = 0; j < 4; j++)
            headerStr += ";" + Settings::functionNames[i] + " ( " + Settings::arrayTypes[j] + " )";

    string resultStr = headerStr + "\n";

    // Размерность [50, 300] с шагом 50
    for (auto i = 50; i <= 300; i += 50)
        resultStr += countForLength(i) + "\n";

    writeToFile("result1.csv", resultStr);

    resultStr = headerStr + "\n";

    // Размерность [100, 4100] с шагом 1000
    for (auto i = 100; i <= 4100; i += 1000)
        resultStr += countForLength(i) + "\n";

    writeToFile("result2.csv", resultStr);

    Settings::free();
}

