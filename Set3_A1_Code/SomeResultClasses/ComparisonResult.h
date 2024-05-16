#ifndef A1_COMPARISONRESULT_H
#define A1_COMPARISONRESULT_H
#include <cstdint>
#include <cstdio>
#include <chrono>

/// Структура, хранящая результаты сортировки
/// Имеет перегрузку на вывод в поток std::ostream
struct ComparisonResult {
    ComparisonResult(size_t s, std::chrono::microseconds t, size_t comp_num) {
        size = s;
        time = t;
        comparisons_num = comp_num;
    }

    size_t size;
    std::chrono::microseconds time;
    size_t comparisons_num;
};

std::ostream &operator<<(std::ostream &stream, const ComparisonResult &result) {
    stream << result.size << ";" << result.time.count() << ";" << result.comparisons_num;
    return stream;
}

#endif //A1_COMPARISONRESULT_H
