#include <iostream>
#include "Graphics/WndCreator.hpp"
#include "Graphics/GdiPP.hpp"
#include <thread>

int sx = GetSystemMetrics(SM_CXSCREEN);
int sy = GetSystemMetrics(SM_CYSCREEN);

std::atomic_bool Stop = false;

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

		return pivotIndex;
	}

	void Qs(int arr[], int start, int end)
	{
		if (start >= end || Stop == true)
			return;

		int p = partition(arr, start, end);

		Sleep(1);

		Qs(arr, start, p - 1);

		Qs(arr, p + 1, end);
	}

	void QuickSort(int Arr[], int n)
	{
		Qs(Arr, 0, n - 1);
	}
}


int main()
{
	srand(time(NULL) + rand());
	std::cout << "Screen Dims: " << sx << ", " << sy << std::endl;
	std::cout << "Array Size (Max: " << sx << "): ";
	int ArrSz = 0;
	std::cin >> ArrSz;

	int* Array = new int[ArrSz];

	// Create Window - FullScreen Borderless - Get Screen HW - ScreenWidth / elements - generate arr by using random between 1 and height

	RandomizeArr(Array, ArrSz);

	BrushPP ClearBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndCreator Wnd = WndCreator(CS_OWNDC, L"GameEngine", L"Game Engine", LoadCursor(NULL, IDC_ARROW), NULL, ClearBrush, WndExModes::BorderLessEx, WndModes::BorderLess | WndModes::ClipChildren, 0, 0, sx, sy);
	GdiPP Gdi = GdiPP(Wnd.Wnd, true);

	MSG msg;

	std::thread T;
	bool Running = false;
	std::string AlgoNames[] = { "Selection", "Bubble", "Quick"};
	void (*FuncPointers[])(int*, int) = { SortFuncs::SelectionSort, SortFuncs::BubbleSort, SortFuncs::QuickSort};
	int SortingAlgo = 0;

	Sleep(500);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE);

		// Translate and Dispatch message to WindowProc
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

		// Check Msg
		if (msg.message == WM_QUIT || msg.message == WM_CLOSE || msg.message == WM_DESTROY)
		{
			break;
		}

		if (GetAsyncKeyState(VK_RETURN))
		{
			if (!Running)
			{
				//T = std::thread(qsort, Array, ArrSz, sizeof(int), compare);
				T = std::thread(FuncPointers[SortingAlgo], Array, ArrSz);
				Running = true;
			}
		}

		if (GetAsyncKeyState(VK_F1))
		{
			if (Running == true)
			{
				Stop = true;

				if (T.joinable())
					T.join();

				Stop = false;
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

		if (GetAsyncKeyState(VK_BACK))
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

		Gdi.DrawStringA(20, 40, RenderedStr, RGB(255, 0, 0), TRANSPARENT);
		Gdi.DrawStringA(20, 20, AlgoStr, RGB(255, 0, 0), TRANSPARENT);

		Gdi.DrawDoubleBuffer();
	}

	if (Running)
	{
		T.join();
	}
}