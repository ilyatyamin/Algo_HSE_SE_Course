#include "StringGenerator.h"
#include <string>
#include <random>

std::string StringGenerator::GenerateString(int left_border, int right_border) {
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<size_t> distrib(left_border, right_border);
    std::uniform_int_distribution<size_t> alph_distrib(0, alphabet.size() - 1);

    size_t len = distrib(gen);

    std::string result;
    for (size_t i = 0; i < len; ++i) {
        result += alphabet[alph_distrib(gen)];
    }
    return result;
}

std::vector<std::string> StringGenerator::GenerateSetOfStrings(size_t vector_size) {
    std::vector<std::string> result(vector_size);
    for (size_t i = 0; i < vector_size; ++i) {
        result[i] = GenerateString();
    }
    return result;
}
