#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>
#include "mergeSort.cpp"
#include "mergeInsertionSort.cpp"

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> distrib(0, 3001);

std::vector<int> GenerateRandomVector(size_t size) {
    std::vector<int> vector;
    vector.reserve(size);
    for (int i = 0; i < size; ++i) {
        vector.push_back(distrib(gen));
    }
    return vector;
}

void WriteVectorToFile(std::vector<int>& vector, const char* nameFile) {
    std::ofstream out;
    out.open(nameFile);
    for (const auto& element : vector) {
        out << element << " ";
    }
    out.close();
}

void Test(std::vector<int>& group1, int groupNumber, const char* fileName) {
    std::ofstream out;
    out.open(fileName);

    for (int i = 499; i < 4000; i += 100) {
        std::vector<int> test = std::vector<int>(group1.begin(), group1.begin() + i);

        auto start = std::chrono::steady_clock::now();
        MergeSort(test, 0, test.size() - 1);
        auto end = std::chrono::steady_clock::now();
        double millisec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

        out << i+1 << ";" << millisec << "\n";
    }
    out.close();
}

void TestMergeInsertion(std::vector<int> group1, int groupNumber, const char* fileName) {
    std::ofstream out;
    out.open(fileName);

    std::vector<int> borders{5, 10, 20, 50};
    for (const auto& bord : borders) {
        auto start = std::chrono::steady_clock::now();
        MergeInsertionSort(group1, 0, group1.size() - 1, bord);
        auto end = std::chrono::steady_clock::now();
        double millisec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

        out << bord << ";" << millisec << "\n";
    }

    out.close();
}


int main() {
    std::vector<int> group1{GenerateRandomVector(4000)}; // 4000 randomized numbers

    std::vector<int> group2{GenerateRandomVector(4000)}; // sort by descending
    std::sort(group2.begin(), group2.end(), std::greater());

    std::vector<int> group3{GenerateRandomVector(4000)}; // almost sorted (50 swapped elements)
    std::sort(group3.begin(), group3.end());
    for (int i = 0; i < 50; ++i) {
        std::swap(group3[distrib(gen)], group3[distrib(gen)]);
    }

    WriteVectorToFile(group1, "data1.txt");
    WriteVectorToFile(group2, "data2.txt");
    WriteVectorToFile(group3, "data3.txt");

    Test(group1, 1, "test_merge_1.txt");
    Test(group2, 2, "test_merge_2.txt");
    Test(group3, 3, "test_merge_3.txt");

    TestMergeInsertion(group1, 1, "test_mergeInsertion_1.txt");
    TestMergeInsertion(group2, 2, "test_mergeInsertion_2.txt");
    TestMergeInsertion(group3, 3, "test_mergeInsertion_3.txt");

    return 0;
}
