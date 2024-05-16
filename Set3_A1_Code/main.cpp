#include <vector>
#include <string>
#include "TaskHelper.h"
#include "StringSorts.cpp"
#include "SomeResultClasses/ComparisonResult.h"

int main() {
    //TaskHelper::RegenerateDataForTask();
    std::vector<std::string> types{"sorted_", "random_", "almostSorted_"};
    StringSorts sorts;

    for (const auto &type: types) {
        std::vector<ComparisonResult> quick_sort;
        std::vector<ComparisonResult> merge_sort;
        std::vector<ComparisonResult> quick_specific;
        std::vector<ComparisonResult> merge_specific;
        std::vector<ComparisonResult> msd_without;
        std::vector<ComparisonResult> msd_with_quick;

        for (int size = 100; size <= 10000; size += 100) {
            std::string path = "data/" + type + std::to_string(size) + ".txt";
            auto data = TaskHelper::ReadFromFile(path);

//            for (int i = 0; i < 5; ++i) {
                TaskHelper::FillResult(data, size, quick_sort, sorts, SorterType::BasicQuick);
                TaskHelper::FillResult(data, size, merge_sort, sorts, SorterType::BasicMerge);
                TaskHelper::FillResult(data, size, quick_specific, sorts, SorterType::SpecialQuick);
                TaskHelper::FillResult(data, size, merge_specific, sorts, SorterType::SpecialMerge);
                TaskHelper::FillResult(data, size, msd_without, sorts, SorterType::MSD_WithoutQuick);
                TaskHelper::FillResult(data, size, msd_with_quick, sorts, SorterType::MSD_Quick);
//            }

        }

        TaskHelper::WriteResultsToFile("results/" + type + "basic_quicksort.txt", quick_sort);
        TaskHelper::WriteResultsToFile("results/" + type + "basic_mergesort.txt", merge_sort);
        TaskHelper::WriteResultsToFile("results/" + type + "specific_quicksort.txt", quick_specific);
        TaskHelper::WriteResultsToFile("results/" + type + "specific_merge.txt", merge_specific);
        TaskHelper::WriteResultsToFile("results/" + type + "msd_withoutquick.txt", msd_without);
        TaskHelper::WriteResultsToFile("results/" + type + "msd_withtquick.txt", msd_with_quick);
    }

    return 0;
}
