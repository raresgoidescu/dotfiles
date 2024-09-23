bool get_comparison(int a, int b) {
    return ;
}

int make_partition(std::vector<int> &arr, int l, int r) {
	int pivot = arr[r];

	int i = l - 1;

	for (int j = l; j < r ; ++j) {
		if (get_comparison(arr[j], pivot)) {
			++i;
			std::swap(arr[i], arr[j]);
		}
	}

	std::swap(arr[++i], arr[r]);

	return i;
}

void quick_sort(std::vector<int> &arr, int l, int r) {
	if (l < r) {
		int piv = make_partition(arr, l, r);
		quick_sort(arr, l, piv - 1);
		quick_sort(arr, piv + 1, r);
	}
}
