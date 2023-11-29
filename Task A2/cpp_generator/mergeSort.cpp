void MergeTwoVectors(std::vector<int> &vector, int left, int middle, int right) {
    std::vector<int> answer;

    auto ptr1 = left;
    auto ptr2 = middle + 1;

    while (ptr1 != middle + 1 && ptr2 != right + 1) {
        if (vector[ptr1] < vector[ptr2]) {
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
    for (const auto& el : answer) {
        vector[i] = el;
        ++i;
    }
}

void MergeSort(std::vector<int>& list, int left, int right) {
    if (right <= left) {
        return;
    }

    int middle = (left + right) / 2;

    MergeSort(list, left, middle);
    MergeSort(list, middle +1, right);
    MergeTwoVectors(list, left, middle, right);
}