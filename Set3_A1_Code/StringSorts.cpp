#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <set>

#pragma once

/// Класс сортировок, который реализует все сортировки, необходимые в задании
/// Тут много всего непонятного, я постараюсь комментариями все пояснить
class StringSorts {
public:
    /// @return Счетчик сравнений для какой-либо сравнений
    /// Каждая сортировка в процессе сравнений увеличивает данный счетчик
    /// Изначально я планировал распараллелить все это, поэтому в коде могут оставаться намеки на многопоточку
    [[nodiscard]] size_t GetComparisonCounter() const {
        return comparison_counter;
    }

    /// @result Очищает счетчик сравнений. Вызывается методами после завершения сортировки
    void ResetComparisonCounter() {
        comparison_counter = 0;
    }

    /// Стандартный Quick Sort (inplace) без специализации для строк.
    template<typename T>
    void StandardQuickSort(std::vector<T> &mas, int lborder, int rborder) {
        if (rborder > lborder) {
            int pivot = RandomizedPartition(mas, lborder, rborder);
            StandardQuickSort(mas, lborder, pivot - 1);
            StandardQuickSort(mas, pivot + 1, rborder);
        }
    }

    /// Стандартный Merge Sort (inplace) без специализации для строк.
    template<typename T>
    void StandardMergeSort(std::vector<T> &list, int left, int right) {
        if (right <= left) {
            return;
        }

        int middle = (left + right) / 2;

        StandardMergeSort(list, left, middle);
        StandardMergeSort(list, middle + 1, right);
        MergeTwoVectors(list, left, middle, right);
    }

    /// Специфический String-QuickSort.
    /// @var T - типиизированный параметр, так было удобно.
    /// @params data - объект типа std::vector из строк.
    /// @params common - длина общего префикса. По умолчанию, равен 0.
    template<typename T>
    std::vector<T> SpecificQuickSort(std::vector<T> &data, int common = 0) {
        if (data.size() <= 1) {
            return data;
        }

        auto R_excl = std::vector<T>();
        for (T &element: data) {
            if (element.size() == common) {
                R_excl.emplace_back(std::move(element));
            }
        }

        if (R_excl.size() == data.size()) {
            return R_excl;
        }

        int rborder = data.size() - 1;
        int lborder = 0;
        int pivotIdx = rand() % (rborder - lborder + 1) + lborder;
        auto pivot = data[pivotIdx];

        std::vector<T> R_less;
        std::vector<T> R_equal;
        std::vector<T> R_great;
        R_less.reserve(data.size());
        R_equal.reserve(data.size());
        R_great.reserve(data.size());

        for (T &element: data) {
            if (element[common] < pivot[common]) {
                ++comparison_counter;
                R_less.emplace_back(std::move(element));
            } else if (element[common] == pivot[common]) {
                ++comparison_counter;
                R_equal.emplace_back(std::move(element));
            } else {
                R_great.emplace_back(std::move(element));
            }
        }

        R_less = SpecificQuickSort(R_less, common);
        R_equal = SpecificQuickSort(R_equal, common + 1);
        R_great = SpecificQuickSort(R_great, common);

        std::vector<T> result;
        result.reserve(R_excl.size() + R_less.size() + R_equal.size() + R_great.size());

        // тут я использую move-итераторы, чтобы быстрее все работало, так как нам векторы эти в задании не нужны потом
        result.insert(result.end(), std::make_move_iterator(R_excl.begin()), std::make_move_iterator(R_excl.end()));
        result.insert(result.end(), std::make_move_iterator(R_less.begin()), std::make_move_iterator(R_less.end()));
        result.insert(result.end(), std::make_move_iterator(R_equal.begin()), std::make_move_iterator(R_equal.end()));
        result.insert(result.end(), std::make_move_iterator(R_great.begin()), std::make_move_iterator(R_great.end()));

        return result;
    }

    /// Специфический String-MergeSort.
    /// @var T - типиизированный параметр, так было удобно.
    /// @params data - объект типа std::vector из строк.
    template<typename T>
    std::vector<T> SpecificMergeSort( std::vector<T> &data) {
        std::vector<std::pair<T, int>> result = SpecificMergeSort_Inner(data);
        std::vector<T> without_pairs;
        without_pairs.reserve(result.size());
        std::transform(result.begin(), result.end(), std::back_inserter(without_pairs),
                       [](const std::pair<T, int> &p) { return p.first; });

        return without_pairs;
    }

    /// Специфический String-MSDRadix без переключения на QuickSort.
    /// @var T - типиизированный параметр, так было удобно.
    /// @params data - объект типа std::vector из строк.
    template<typename T>
    std::vector<T> SpecificMSDRadix_WithoutQuick( std::vector<T> &data) {
        // Посчитаем m, длину алфавита
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#%:;^&*()-";

        return SpecificMSDRadix_WithoutQuick_Inner(data, alphabet.size(), 0);
    }

    /// Специфический String-MSDRadix с переключением на QuickSort.
    /// @var T - типиизированный параметр, так было удобно.
    /// @params data - объект типа std::vector из строк.
    template<typename T>
    std::vector<T> SpecificMSDRadix_WithQuick( std::vector<T> &data) {
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#%:;^&*()-";

        return SpecificMSDRadix_WithQuick_Inner(data, alphabet.size(), 0);
    }


private:
    size_t comparison_counter = 0;

    /// Возвращает длину общего префикса двух строк
    /// Используется специализированными алгоритмами для быстрого сравнения строк
    size_t LCP(const std::string &str1, const std::string &str2) {
        for (int i = 0; i < std::min(str1.size(), str2.size()); ++i) {
            ++comparison_counter;
            if (str1[i] != str2[i]) {
                return i;
            }
        }
        return std::min(str1.size(), str2.size()) - 1;
    }

    /// Возвращает {-1, 0, 1}
    /// -1, если первая строка лексикографически меньше второй
    /// 0, если первая строка равна второй
    /// 1, если первая строка больше второй
    /// Используется неспециализированными алгоритмами с целью подсчета количества сравниваемых символов
    int Compare(const std::string &str1, const std::string &str2) {
        for (int i = 0; i < std::min(str1.size(), str2.size()); ++i) {
            ++comparison_counter;
            if (str1[i] != str2[i]) {
                if (str1[i] > str2[i]) {
                    return 1;
                }
                if (str1[i] < str2[i]) {
                    return -1;
                }
            }
        }
        if (str1.size() != str2.size()) {
            if (str1.size() < str2.size()) {
                return -1;
            }
            if (str1.size() > str2.size()) {
                return 1;
            }
        }
        return 0;
    }

    /// Внутренняя реализация MergeSort
    /// Используется public-методом SpecificMergeSort
    /// @params data - вектор из строк
    template<typename T>
    std::vector<std::pair<T, int>> SpecificMergeSort_Inner(const std::vector<T> &data) {
        if (data.size() == 1) {
            return {{data[0], 0}};
        }

        int m = data.size() / 2;

        auto p = SpecificMergeSort_Inner(std::vector<T>(data.begin(), data.begin() + m));
        auto q = SpecificMergeSort_Inner(std::vector<T>(data.begin() + m, data.end()));

        return StringMerge(p, q);
    }

    /// Внутренняя реализация MSD Radix без переключения на QuickSort
    /// Используется public-методом SpecificMSDRadix_WithoutQuick
    /// @params data - вектор из строк
    /// @params indexer - словарь, где ключем является символ, а значением - положение этого словаря в векторе в CountingSort
    /// @params m - мощность множества алфавита строк
    /// @params L - текущая длина общего префикса всех строк
    template<typename T>
    std::vector<T> SpecificMSDRadix_WithoutQuick_Inner( std::vector<T> &data,
                                                       size_t m,
                                                       size_t L) {
        if (data.size() <= 1) {
            return data;
        }
        if (data.size() == 2 && data[0] == data[1]) {
            return data;
        }

        std::vector<T> R_excl;
        for (const T &str: data) {
            if (str.size() == L) {
                R_excl.emplace_back(str);
            }
        }

        // Посчитаем длину общего префикса
        size_t common_prefix = std::numeric_limits<size_t>::max();
        for (int i = 1; i < data.size(); ++i) {
            common_prefix = std::min(common_prefix, LCP(data[0], data[i]));
        }

        std::vector<std::vector<std::string>> counters = CountingSort(data, m, common_prefix);

        int value = 0;
        for (auto &counter: counters) {
            counter = SpecificMSDRadix_WithoutQuick_Inner(counter, m, common_prefix);
            value += counter.size();
        }

        std::vector<T> result;
        result.reserve(value + R_excl.size());
        result.insert(result.end(), R_excl.begin(), R_excl.end());

        for (auto &counter: counters) {
            result.insert(result.end(), counter.begin(), counter.end());
        }
        return result;
    }

    /// Внутренняя реализация MSD Radix c переключением на QuickSort
    /// Используется public-методом SpecificMSDRadix_WithQuick
    /// @params data - вектор из строк
    /// @params indexer - словарь, где ключем является символ, а значением - положение этого словаря в векторе в CountingSort
    /// @params m - мощность множества алфавита строк
    /// @params L - текущая длина общего префикса всех строк
    template<typename T>
    std::vector<T> SpecificMSDRadix_WithQuick_Inner(std::vector<T> &data,
                                                    size_t m,
                                                    size_t L) {
        if (data.size() < m) {
            return SpecificQuickSort(data);
        }
        if (data.size() == 2 && data[0] == data[1]) {
            return data;
        }

        std::vector<T> R_excl;
        for (const T &str: data) {
            if (str.size() == L) {
                R_excl.emplace_back(str);
            }
        }

        // Посчитаем длину общего префикса
        size_t common_prefix = std::numeric_limits<size_t>::max();
        for (int i = 1; i < data.size(); ++i) {
            common_prefix = std::min(common_prefix, LCP(data[0], data[i]));
        }

        std::vector<std::vector<std::string>> counters = CountingSort(data, m, common_prefix);

        size_t value = 0;
        for (auto & counter : counters) {
            counter = SpecificMSDRadix_WithQuick_Inner(counter, m, common_prefix);
            value += counter.size();
        }

        std::vector<T> result;
        result.reserve(value + R_excl.size());
        result.insert(result.end(), R_excl.begin(), R_excl.end());

        for (auto &counter: counters) {
            result.insert(result.end(), counter.begin(), counter.end());
        }
        return result;
    }

    /// Внутренняя реализация Counting Sort, используемая MSD-алгоритмами
    /// @params data - вектор из строк
    /// @params m - мощность множества алфавита строк
    /// @params L - текущая длина общего префикса всех строк
    std::vector<std::vector<std::string>> CountingSort(std::vector<std::string> &data,
                                                       size_t M,
                                                       size_t L) {
        std::vector<std::vector<std::string>> result(128, std::vector<std::string>());

        for (const std::string &str: data) {
            result[int(str[L])].emplace_back(str);
        }

        return result;
    }

    /// Сравнение двух строк с "подсказкой" k - длиной общего префикса
    /// @params str1 - первая строка
    /// @params str2 - вторая строка
    /// @params k - подсказка, то есть длина общего префикса
    std::pair<int, int> LCPCompare( std::string &str1,
                                    std::string &str2,
                                   int k) {
        for (int i = k; i < std::min(str1.size(), str2.size()); ++i) {
            if (str1[i] < str2[i]) {
                ++comparison_counter;
                return {-1, i};
            } else if (str1[i] > str2[i]) {
                comparison_counter += 2;
                return {1, i};
            }
        }
        if (str1.size() == str2.size()) {
            return {0, str1.size() - 1};
        }
        return {str1.size() < str2.size(), std::min(str1.size(), str2.size())};
    }

    /// Мёрджит два вектора пар для специфичного алгоритма Merge Sort (String realization)
    template<typename T>
    std::vector<std::pair<T, int>> StringMerge(std::vector<std::pair<T, int>> &data1,
                                               std::vector<std::pair<T, int>> &data2) {
        std::vector<std::pair<T, int>> result;

        int i = 0;
        int j = 0;

        while (i < data1.size() && j < data2.size()) {
            if (data1[i].second > data2[j].second) {
                result.emplace_back(data1[i]);
                ++i;
            } else if (data1[i].second < data2[j].second) {
                result.emplace_back(data2[j]);
                ++j;
            } else {
                auto elem = LCPCompare(data1[i].first, data2[j].first, data1[i].second);
                if (elem.first == -1) {
                    result.emplace_back(data1[i]);
                    ++i;
                    data2[j].second = elem.second;
                } else {
                    result.emplace_back(data2[j]);
                    ++j;
                    data1[i].second = elem.second;
                }
            }
        }

        while (i < data1.size()) {
            result.emplace_back(data1[i]);
            ++i;
        }

        while (j < data2.size()) {
            result.emplace_back(data2[j]);
            ++j;
        }
        return result;
    }

    /// Внутренняя реализация метода Partition для обычного QuickSort
    /// Использует метод Compare для подсчета количества сравниваемых символов
    /// @params mas - вектор строк
    /// @params lborder - левая граница, на которой происходит Partition
    /// @params rborder - правая граница, на которой происходит Partition
    template<typename T>
    int Partition(std::vector<T> &mas, int lborder, int rborder) {
        T first = mas[rborder];
        int min_elem = lborder;

        for (size_t i = lborder; i < rborder; ++i) {
            if ((Compare(mas[i], first) == -1)) {
                std::swap(mas[i], mas[min_elem]);
                ++min_elem;
            }
        }
        std::swap(mas[min_elem], mas[rborder]);

        return min_elem;
    }

    /// Рандомно выбирает Partition и начинает разделение
    /// Используется в обычном (неспециализированном) алгоритме Quick Sort
    template<typename T>
    int RandomizedPartition(std::vector<T> &vect, int lborder, int rborder) {
        int pivotIndex = rand() % (rborder - lborder + 1) + lborder;
        std::swap(vect[pivotIndex], vect[rborder]);
        return Partition(vect, lborder, rborder);
    }

    /// Мёрджит два вектора, имея указатель на начало первого, на конец первого и на конец второго
    template<typename T>
    void MergeTwoVectors(std::vector<T> &vector, int left, int middle, int right) {
        std::vector<T> answer;

        auto ptr1 = left;
        auto ptr2 = middle + 1;

        while (ptr1 != middle + 1 && ptr2 != right + 1) {
            if (Compare(vector[ptr1], vector[ptr2]) == -1) {
                answer.emplace_back(vector[ptr1]);
                ++ptr1;
            } else {
                answer.emplace_back(vector[ptr2]);
                ++ptr2;
            }
        }

        // Move any remaining elements from either list
        while (ptr1 != middle + 1) {
            answer.emplace_back(vector[ptr1]);
            ++ptr1;
        }
        while (ptr2 != right + 1) {
            answer.emplace_back(vector[ptr2]);
            ++ptr2;
        }

        int i = left;
        for (const auto &el: answer) {
            vector[i] = el;
            ++i;
        }
    }
};
