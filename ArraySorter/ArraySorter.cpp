#include <iostream>
#include "Graphics/WndCreator.hpp"
#include "Graphics/GdiPP.hpp"
#include <thread>

int sx = GetSystemMetrics(SM_CXSCREEN);
int sy = GetSystemMetrics(SM_CYSCREEN);

std::atomic_bool Stop = false;
std::atomic_bool Finished = false;

int RandInt(int min, int max)
{
	int Range = max - min + 1;
	return rand() % Range + min;
}

void RandomizeArr(int* Arr, int n)
{
	srand(time(NULL) + rand());
	for (int i = 0; i < n; i++)
	{
		Arr[i] = RandInt(1, sy);
	}
}

namespace SortFuncs
{
	void SelectionSort(int arr[], int n)
	{
		int i, j, min_idx;

		for (i = 0; i < n - 1; i++)
		{
			if (Stop == true)
				return;

			min_idx = i;
			for (j = i + 1; j < n; j++) {
				if (arr[j] < arr[min_idx])
					min_idx = j;
			}

			if (min_idx != i)
				std::swap(arr[min_idx], arr[i]);

			Sleep(1);
		}

		Finished = true;
	}

	void BubbleSort(int arr[], int n)
	{
		int i, j;
		bool swapped;
		for (i = 0; i < n - 1; i++)
		{
			if (Stop == true)
				return;
			swapped = false;
			for (j = 0; j < n - i - 1; j++) {
				if (arr[j] > arr[j + 1]) {
					std::swap(arr[j], arr[j + 1]);
					swapped = true;
				}
			}

			if (swapped == false)
				break;
			Sleep(1);
		}
		Finished = true;
	}


	int partition(int arr[], int start, int end)
	{

		int pivot = arr[start];

		int count = 0;
		for (int i = start + 1; i <= end; i++) {
			if (arr[i] <= pivot)
				count++;
		}

		// Giving pivot element its correct position
		int pivotIndex = start + count;
		std::swap(arr[pivotIndex], arr[start]);

		// Sorting left and right parts of the pivot element
		int i = start, j = end;

		while (i < pivotIndex && j > pivotIndex) {

			while (arr[i] <= pivot) {
				i++;
			}

			while (arr[j] > pivot) {
				j--;
			}

			if (i < pivotIndex && j > pivotIndex) {
				std::swap(arr[i++], arr[j--]);
			}
		}
		Sleep(1);

		return pivotIndex;
	}

	void Qs(int arr[], int start, int end)
	{
		if (start >= end || Stop == true)
			return;

		int p = partition(arr, start, end);

		Qs(arr, start, p - 1);

		Qs(arr, p + 1, end);
	}

	void QuickSort(int Arr[], int n)
	{
		Qs(Arr, 0, n - 1);

		Finished = true;
	}

	void Merge(int array[], int const left, int const mid, int const right)
	{
		int const subArrayOne = mid - left + 1;
		int const subArrayTwo = right - mid;

		// Create temp arrays
		auto* leftArray = new int[subArrayOne],
			* rightArray = new int[subArrayTwo];

		// Copy data to temp arrays leftArray[] and rightArray[]
		for (auto i = 0; i < subArrayOne; i++)
			leftArray[i] = array[left + i];
		for (auto j = 0; j < subArrayTwo; j++)
			rightArray[j] = array[mid + 1 + j];

		auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
		int indexOfMergedArray = left;

		// Merge the temp arrays back into array[left..right]
		while (indexOfSubArrayOne < subArrayOne
			&& indexOfSubArrayTwo < subArrayTwo) {
			if (leftArray[indexOfSubArrayOne]
				<= rightArray[indexOfSubArrayTwo]) {
				array[indexOfMergedArray]
					= leftArray[indexOfSubArrayOne];
				indexOfSubArrayOne++;
			}
			else {
				array[indexOfMergedArray]
					= rightArray[indexOfSubArrayTwo];
				indexOfSubArrayTwo++;
			}
			indexOfMergedArray++;
		}

		// Copy the remaining elements of
		// left[], if there are any
		while (indexOfSubArrayOne < subArrayOne) {
			array[indexOfMergedArray]
				= leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
			indexOfMergedArray++;
		}

		// Copy the remaining elements of
		// right[], if there are any
		while (indexOfSubArrayTwo < subArrayTwo) {
			array[indexOfMergedArray]
				= rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
			indexOfMergedArray++;
		}
		delete[] leftArray;
		delete[] rightArray;
		Sleep(1);
	}

	void Ms(int array[], int const begin, int const end)
	{
		if (begin >= end || Stop)
			return;

		int mid = begin + (end - begin) / 2;
		Ms(array, begin, mid);
		Ms(array, mid + 1, end);
		Merge(array, begin, mid, end);
	}

	void MergeSort(int Arr[], int n)
	{
		Ms(Arr, 0, n);
		Finished = true;
	}

	void InsertionSort(int arr[], int n)
	{
		int i, key, j;
		for (i = 1; i < n; i++) 
		{
			if (Stop)
				return;

			key = arr[i];
			j = i - 1;

			// Move elements of arr[0..i-1],
			// that are greater than key,
			// to one position ahead of their
			// current position
			while (j >= 0 && arr[j] > key) {
				arr[j + 1] = arr[j];
				j = j - 1;
			}
			arr[j + 1] = key;
			Sleep(1);
		}

		Finished = true;
	}

	void Heapify(int Arr[], int n, int i)
	{
		int largest = i; // Initialize largest as root Since we are using 0 based indexing
		int l = 2 * i + 1; // left = 2*i + 1
		int r = 2 * i + 2; // right = 2*i + 2

		// If left child is larger than root
		if (l < n && Arr[l] > Arr[largest])
			largest = l;

		// If right child is larger than largest so far
		if (r < n && Arr[r] > Arr[largest])
			largest = r;

		// If largest is not root
		if (largest != i) {
			std::swap(Arr[i], Arr[largest]);

			// Recursively heapify the affected sub-tree
			Heapify(Arr, n, largest);
		}
	}

	// main function to do heap sort
	void HeapSort(int Arr[], int n)
	{
		// Build heap (rearrange array)
		for (int i = n / 2 - 1; i >= 0; i--)
		{
			if (Stop)
				return;
			Heapify(Arr, n, i);
			Sleep(1);
		}
		// One by one extract an element from heap
		for (int i = n - 1; i >= 0; i--) 
		{
			if (Stop)
				return;
			// Move current root to end
			std::swap(Arr[0], Arr[i]);

			// call max heapify on the reduced heap
			Heapify(Arr, i, 0);
			Sleep(1);
		}

		Finished = true;
	}

	void Ss(int Arr[], int n)
	{
		for (int gap = n / 2; gap > 0; gap /= 2)
		{
			for (int i = gap; i < n; i += 1)
			{
				if (Stop)
					return;
				// add a[i] to the elements that have been gap sorted
				// save a[i] in temp and make a hole at position i
				int temp = Arr[i];

				// shift earlier gap-sorted elements up until the correct 
				// location for a[i] is found
				int j;
				for (j = i; j >= gap && Arr[j - gap] > temp; j -= gap)
					Arr[j] = Arr[j - gap];

				//  put temp (the original a[i]) in its correct location
				Arr[j] = temp;
				Sleep(1);
			}
		}
		return;
	}

	void ShellSort(int Arr[], int n)
	{
		Ss(Arr, n);
		Finished = true;
	}
}


int main()
{
	int ArrSz = 0;

	std::cout << "Screen Dims: " << sx << ", " << sy << std::endl;
	std::cout << "Array Size (Max: " << sx << "): ";

	std::cin >> ArrSz;

	Sleep(700);

	int* Array = new int[ArrSz];

	RandomizeArr(Array, ArrSz);

	BrushPP ClearBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndCreator Wnd = WndCreator(CS_OWNDC, L"GameEngine", L"Game Engine", LoadCursor(NULL, IDC_ARROW), NULL, ClearBrush, WndExModes::BorderLessEx, WndModes::BorderLess | WndModes::ClipChildren, 0, 0, sx, sy);
	GdiPP Gdi = GdiPP(Wnd.Wnd, true);

	MSG msg;

	std::thread T;
	bool Running = false;
	bool DoRand = false;
	bool DoTime = false;
	double TimeToSort = 0;
	double ElapsedTime = 0.00;

	std::string AlgoNames[] = { "Selection", "Bubble", "Quick", "Merge", "Insertion", "Heap", "Shell" };
	void (*FuncPointers[])(int*, int) = { SortFuncs::SelectionSort, SortFuncs::BubbleSort, SortFuncs::QuickSort, SortFuncs::MergeSort, SortFuncs::InsertionSort, SortFuncs::HeapSort, SortFuncs::ShellSort };
	int SortingAlgo = 0;

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		auto Start = std::chrono::system_clock::now();

		PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE);

		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		if (msg.message == WM_QUIT || msg.message == WM_CLOSE || msg.message == WM_DESTROY)
		{
			break;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (DoRand)
			{
				RandomizeArr(Array, ArrSz);
				DoRand = false;
			}

			if (!Running)
			{
				TimeToSort = 0.00;
				//T = std::thread(qsort, Array, ArrSz, sizeof(int), compare);
				T = std::thread(FuncPointers[SortingAlgo], Array, ArrSz);
				Running = true;
				DoTime = true;
			}
		}

		if (GetAsyncKeyState(VK_F1) & 0x8000)
		{
			if (Running == true)
			{
				Stop = true;

				if (T.joinable())
					T.join();

				Stop = false;
				DoTime = false;
				Running = false;
				RandomizeArr(Array, ArrSz);
			}
			if (SortingAlgo < (sizeof(FuncPointers) / sizeof(void*)) - 1)
			{
				SortingAlgo++;
			}
			else
			{
				SortingAlgo = 0;
			}
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_BACK) & 0x8000)
		{
			if (Running == false)
			{
				RandomizeArr(Array, ArrSz);
			}
		}

		Gdi.Clear();

		int x = 0;
		int RenderedElements = 0;
		
		for (int i = 0; i < ArrSz; i++)
		{
			int Rounded = std::roundf((float)sx / (float)ArrSz);

			if (Rounded < 1)
				Rounded += 1;

			Gdi.DrawFilledRect(x, sy - Array[i], Rounded, Array[i], BrushPP(RGB(255, 255, 255)));

			x += Rounded;

			RenderedElements = i;

			if (x > sx)
				break;

		}

		std::string AlgoStr = "Sorting Algorithm: " + AlgoNames[SortingAlgo];
		std::string RenderedStr = "Rendered Elements: " + std::to_string(RenderedElements);
		std::string Stat = Running ? "Running" : "Not Running";
		std::string ThreadStat = "Sorting Thread: " + Stat;
		std::string TimeStr = "Time: " + std::to_string(TimeToSort) + " Sec";

		Gdi.DrawStringA(20, 10, "Array Sorter By Griffin Smith", RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 25, AlgoStr, RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 40, RenderedStr, RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 55, ThreadStat, RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 70, TimeStr, RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 85, "(ESC) Exit", RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 100, "(F1) Change Algorithm", RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 115, "(Enter) Begin Sort", RGB(0, 255, 255), TRANSPARENT);
		Gdi.DrawStringA(20, 130, "(BackSpace) Randomize", RGB(0, 255, 255), TRANSPARENT);

		Gdi.DrawDoubleBuffer();

		ElapsedTime = std::chrono::duration<double>(std::chrono::system_clock::now() - Start).count();

		if (DoTime)
		{
			TimeToSort += ElapsedTime;
		}

		if (Finished && Running)
		{
			if (T.joinable())
				T.join();

			Running = false;
			Finished = false;

			DoTime = false;
			DoRand = true;
		}
	}

	if (Running)
	{
		Stop = true;
		T.join();
	}

	Wnd.Destroy();
}