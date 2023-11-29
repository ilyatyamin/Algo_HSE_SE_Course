void SelectionSort(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
    for (auto i = begin + 1; i < end; ++i) {
        auto key = *i;
        auto j = i - 1;
        while (j >= begin && *j > key) {
            std::swap(*(j+1), *j);
            j = j - 1;
        }
        *(j + 1) = key;
    }
}

void MergeInsertionSort(std::vector<int>& list, int left, int right, int border) {
    if (right - left <= border) {
        SelectionSort(list.begin() + left, list.begin() + right + 1);
        return;
    }

    int middle = (left + right) / 2;

    MergeInsertionSort(list, left, middle, border);
    MergeInsertionSort(list, middle +1, right, border);
    MergeTwoVectors(list, left, middle, right);
}