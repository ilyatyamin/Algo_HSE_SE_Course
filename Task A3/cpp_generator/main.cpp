#include <iostream>
#include <fstream>
#include <vector>
#include "quicksort.cpp"
#include "quickHeapSort.cpp"


std::vector<int> ReadVectorFromFile(const char* path) {
    std::ifstream file;
    file.open(path);

    std::vector<int> vector;
    vector.reserve(4000);
    for (int i = 0; i < 4'000; ++i) {
        int number;
        file >> number;
        vector.push_back(number);
    }
    file.close();
    return vector;
}


void TestQuick(std::vector<int>& group1, int groupNumber, const char* fileName) {
    std::ofstream out;
    out.open(fileName, std::ios::app);

    for (int i = 499; i < 4000; i += 100) {
        std::vector<int> test = std::vector<int>(group1.begin(), group1.begin() + i);

        auto start = std::chrono::high_resolution_clock::now();
        QuickSort(test, 0, test.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        double millisec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()) / 1000.0;

        out << i+1 << ";" << millisec << "\n";
    }
    out.close();
}

void TestQuickHeap(std::vector<int>& group1, int groupNumber, const char* fileName) {
    std::ofstream out;
    out.open(fileName, std::ios::app);

    std::vector<int> borders{5, 10, 20, 50};

    for (const int& border : borders) {
        for (int i = 499; i < 4000; i += 100) {
            std::vector<int> test = std::vector<int>(group1.begin(), group1.begin() + i);

            auto start = std::chrono::high_resolution_clock::now();
            QuickHeapSort(test, 0, test.size() - 1, border);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            double millisec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()) / 1000.0;

            out << i+1 << ";" << border << ";" << millisec << "\n";
        }
    }

    out.close();
}

int main() {
    auto vector1 = ReadVectorFromFile("/Users/mrshrimp.it/Documents/ВШЭ учеба/2 курс/Алгоритмы/ДЗ/Algorithms_SET_3_HW/Task A2/data1.txt");
    auto vector2 = ReadVectorFromFile("/Users/mrshrimp.it/Documents/ВШЭ учеба/2 курс/Алгоритмы/ДЗ/Algorithms_SET_3_HW/Task A2/data2.txt");
    auto vector3 = ReadVectorFromFile("/Users/mrshrimp.it/Documents/ВШЭ учеба/2 курс/Алгоритмы/ДЗ/Algorithms_SET_3_HW/Task A2/data3.txt");

    // 15 раз, чтобы потом взять среднее
    for (int i = 0; i < 15; ++i) {
        TestQuick(vector1, 1, "quick1.txt");
        TestQuick(vector2, 2, "quick2.txt");
        TestQuick(vector3, 3, "quick3.txt");

        TestQuickHeap(vector1, 1, "quickHeap1.txt");
        TestQuickHeap(vector2, 2, "quickHeap2.txt");
        TestQuickHeap(vector3, 3, "quickHeap3.txt");
    }

    return 0;
}
