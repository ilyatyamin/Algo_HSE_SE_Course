int Partition(std::vector<int> &mas, int lborder, int rborder) {
    int first = mas[rborder];
    int min_elem = lborder;

    for (size_t i = lborder; i < rborder; ++i) {
        if ((mas[i] < first)) {
            std::swap(mas[i], mas[min_elem]);
            ++min_elem;
        }
    }
    std::swap(mas[min_elem], mas[rborder]);

    return min_elem;
}

int RandomizedPartition(std::vector<int>& vect, int lborder, int rborder)
{
    int pivotIndex = rand() % (rborder - lborder + 1) + lborder;
    std::swap(vect[pivotIndex], vect[rborder]);
    return Partition(vect, lborder, rborder);
}


void QuickSort(std::vector<int> &mas, int lborder, int rborder) {
    if (rborder > lborder) {
        // Опорный элемент - последний
        int pivot = RandomizedPartition(mas, lborder, rborder);
        // i - позиция опорного элемента
        QuickSort(mas, lborder, pivot - 1);
        QuickSort(mas, pivot + 1, rborder);
    }
}