#ifndef A1_TASKHELPER_H
#define A1_TASKHELPER_H

#include <fstream>
#include "StringGenerator.h"
#include <random>
#include "StringSorts.cpp"
#include "SomeResultClasses/ComparisonResult.h"

enum TypeOfData {
    Random, Sorted, AlmostSorted
};

enum SorterType {
    BasicQuick, BasicMerge, SpecialQuick, SpecialMerge, MSD_WithoutQuick, MSD_Quick
};

/// Класс, который помогает в выполнении большого количества однотипной работы при выполнении этого задания
class TaskHelper {
public:
    /// Регенирирует все данные для задания.
    /// Создает три типа файлов: с рандомным содержимым, с отсортированным содержимым и с практически отсортированным содержимым
    /// Я немного расширил диапазон рассматриваемых значений до 10'000, с целью показать кое-что в выводе
    static void RegenerateDataForTask() {
        StringGenerator generator;

        for (size_t arr_size = 100; arr_size <= 10000; arr_size += 100) {
            std::vector<std::string> str_arr = generator.GenerateSetOfStrings(arr_size);

            // Case 1. неотсортированный
            TaskHelper::WriteVectorToFile(str_arr, TypeOfData::Random, arr_size);

            // Case 2. отсортированный в обратном
            auto copy2 = str_arr;
            std::sort(copy2.begin(), copy2.end(), std::greater<>());
            TaskHelper::WriteVectorToFile(copy2, TypeOfData::Sorted, arr_size);

            // Case 3. практически отсортировано
            std::sort(copy2.begin(), copy2.end());
            std::random_device device;
            std::mt19937 gen(device());
            std::uniform_int_distribution<size_t> distrib_amount(0, static_cast<size_t>(copy2.size() * 0.02));
            std::uniform_int_distribution<size_t> distrib_elems(0, copy2.size() - 1);

            size_t amount = distrib_amount(gen);
            for (int i = 0; i < amount; ++i) {
                size_t elem1 = distrib_elems(gen);
                size_t elem2 = distrib_elems(gen);

                std::swap(copy2[elem1], copy2[elem2]);
            }
            TaskHelper::WriteVectorToFile(copy2, TypeOfData::AlmostSorted, arr_size);
        }
    }

    /// Пишет результаты эксперимента в файл
    /// @params name - имя файла, в который производится запись
    /// @params result - список из объектов ComparisonResult. Он содержит в себе время сортировки, размер данных и количество сравнений
    static void WriteResultsToFile(const std::string &name,
                                   const std::vector<ComparisonResult> &results) {
        std::ofstream stream;
        stream.open(name);

        for (const auto &element: results) {
            stream << element << "\n";
        }

        stream.close();
    }

    /// Читает данные с файла
    /// @params name - имя файла, с которого производится чтение
    /// @returns вектор из строк
    static std::vector<std::string> ReadFromFile(const std::string &name) {
        std::ifstream stream;
        stream.open(name);

        std::vector<std::string> result;

        std::string str{};
        while (stream >> str) {
            result.emplace_back(str);
        }
        stream.close();

        return result;
    }

    /// Производит эксперимент с сортировкой и записывает результаты в подаваемый вектор
    /// @params data - вектор из строк, который должен быть отсортирован. Передается по значению, чтобы не испортить изначальный список
    /// @params size - размер вектора
    /// @params result - вектор из ComparisonResult, в который надо произвести запись результатов
    /// @params sorts - объект типа StringSorts, содержащий все необходимые сортировки.
    /// @params type - объект типа SorterType (enum), с помощью которого передается тип нужной сортировки
    static void FillResult(std::vector<std::string> data,
                           size_t size,
                           std::vector<ComparisonResult> &result,
                           StringSorts &sorts,
                           SorterType type) {
        sorts.ResetComparisonCounter();

        std::chrono::time_point<std::chrono::high_resolution_clock> begin;
        std::chrono::time_point<std::chrono::high_resolution_clock> end;

        switch (type) {
            case SorterType::BasicQuick:
                begin = std::chrono::high_resolution_clock::now();
                sorts.StandardQuickSort(data, 0, data.size() - 1);
                end = std::chrono::high_resolution_clock::now();
                break;
            case SorterType::BasicMerge:
                begin = std::chrono::high_resolution_clock::now();
                sorts.StandardMergeSort(data, 0, data.size() - 1);
                end = std::chrono::high_resolution_clock::now();
                break;
            case SorterType::SpecialQuick:
                begin = std::chrono::high_resolution_clock::now();
                data = sorts.SpecificQuickSort(data);
                end = std::chrono::high_resolution_clock::now();
                break;
            case SorterType::SpecialMerge:
                begin = std::chrono::high_resolution_clock::now();
                data = sorts.SpecificMergeSort(data);
                end = std::chrono::high_resolution_clock::now();
                break;
            case SorterType::MSD_WithoutQuick:
                begin = std::chrono::high_resolution_clock::now();
                data = sorts.SpecificMSDRadix_WithoutQuick(data);
                end = std::chrono::high_resolution_clock::now();
                break;
            case SorterType::MSD_Quick:
                begin = std::chrono::high_resolution_clock::now();
                data = sorts.SpecificMSDRadix_WithQuick(data);
                end = std::chrono::high_resolution_clock::now();
                break;
        }

        assert(std::is_sorted(data.begin(), data.end())); // is our sort works correctly?
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        result.emplace_back(size, duration, sorts.GetComparisonCounter());
    }

private:
    static void WriteVectorToFile(const std::vector<std::string> &data, TypeOfData type, size_t size_of_arr) {
        std::ofstream stream;

        switch (type) {
            case TypeOfData::Sorted:
                stream.open("data/sorted_" + std::to_string(size_of_arr) + ".txt");
                break;
            case TypeOfData::Random:
                stream.open("data/random_" + std::to_string(size_of_arr) + ".txt");
                break;
            case TypeOfData::AlmostSorted:
                stream.open("data/almostSorted_" + std::to_string(size_of_arr) + ".txt");
                break;
        }

        for (const std::string &str: data) {
            stream << str << "\n";
        }

        stream.close();
    }

};

#endif //A1_TASKHELPER_H
