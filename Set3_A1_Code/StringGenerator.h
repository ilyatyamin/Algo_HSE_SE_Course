#ifndef A1_STRINGGENERATOR_H
#define A1_STRINGGENERATOR_H

#include <string>
#include <vector>


class StringGenerator {
public:
    /// Генерирует строку, состоящую из заданных в алфавите символов
    /// @params left_border - левая граница размера строки
    /// @params right_border - правая граница размера строки
    /// @returns строку случайной длины
    std::string GenerateString(int left_border = 10, int right_border = 200);

    /// Генерирует список строк, заданного размера
    /// @params vector_size - количество элементов в векторе
    /// @returns вектор из строк
    std::vector<std::string> GenerateSetOfStrings(size_t vector_size);

private:
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#%:;^&*()-";
};


#endif //A1_STRINGGENERATOR_H
