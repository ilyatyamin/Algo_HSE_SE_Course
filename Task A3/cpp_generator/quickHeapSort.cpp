void maxHeapify(std::vector<int> &vector, int i, size_t max_size, int lborder) {
    int left = 2 * (i - lborder) +1 + lborder;
    int right = 2 * (i - lborder) + 2 + lborder;

    if (left < max_size && right < max_size) {
        int max_element = vector[left] > vector[right] ? vector[left] : vector[right];
        int iter = vector[left] > vector[right] ? left : right;

        if (vector[i] < max_element) {
            std::swap(vector[i], vector[iter]);
        }
        maxHeapify(vector, iter, max_size, lborder);
    }
    if (left < max_size && right >= max_size) {
        if (vector[i] < vector[left]) {
            std::swap(vector[i], vector[left]);
            return;
        }
    }

    if (left >= max_size && right < max_size) {
        if (vector[i] < vector[right]) {
            std::swap(vector[i], vector[right]);
            return;
        }
    }
}

void buildMaxHeap(std::vector<int> &vector, int lborder, int rborder) {
    for (int i = lborder + static_cast<int>(rborder - lborder - 1) / 2; i >= lborder; --i) {
        maxHeapify(vector, i, rborder, lborder);
    }
}

void HeapSort(std::vector<int> &vector, int lborder, int rborder) {
    buildMaxHeap(vector, lborder, rborder);

    int i = 0;
    for (int j = rborder - 1; j >= lborder + 1; --j) {
        std::swap(vector[j], vector[lborder]);
        ++i;
        maxHeapify(vector, lborder, rborder  - i, lborder);
    }
}

void QuickHeapSort(std::vector<int> &mas, int lborder, int rborder, int borderToHeap) {
    if (lborder < rborder) {
        if (rborder - lborder <= borderToHeap) {
            HeapSort(mas, lborder, rborder + 1);
        } else {
            // Опорный элемент - последний
            int pivot = RandomizedPartition(mas, lborder, rborder);
            // i - позиция опорного элемента
            QuickHeapSort(mas, lborder, pivot - 1, borderToHeap);
            QuickHeapSort(mas, pivot + 1, rborder, borderToHeap);
        }
    }
}