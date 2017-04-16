/* Copyright (c) Lobachevsky University, 2017. All rights reserved.
 * 
 * Radix Sort
 * 
 * Developed by:    Grechukhin Nikita
 *                  Komarov Nikolay
 */

#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

template<class Type>
void createTestData(Type* &data, const int &size)
{
	for (int i = 0; i < size; i++)
	{
		data[i] = rand();
	}
}

template<class Type>
void printResult(Type* &data, const int &size)
{
	std::cout << "Result for " << " :\n";
	for (int i = 0; i < size; i++)
	{
		std::cout << data[i] << "\n";
	}
}

void printTime(int &time32, int &time64)
{
	std::cout << "\nTime for INT-32: " << time32 << " ms\n";
	std::cout << "Time for INT-64: " << time64 << " ms\n";
}

template<class Type>
void CountingSort(Type* inp, Type* out, int byteNum, int size)
{
	unsigned char* mas = reinterpret_cast<unsigned char *>(inp);
	int counter[256];
	int temp;
	memset(counter, 0, sizeof counter);
	for (int i = 0; i < size; i++)
	{
		++counter[mas[sizeof(Type) * i + byteNum]];
	}
	int j = 0;
	for (; j < 256; j++)
	{
		if (counter[j] != 0)
			break;
	}
	temp = counter[j];
	counter[j] = 0;
	j++;
	for (; j < 256; j++)
	{
		int b = counter[j];
		counter[j] = temp;
		temp += b;
	}
	unsigned char c;
	for (int i = 0; i < size; i++)
	{
		c = mas[sizeof(Type) * i + byteNum];
		out[counter[c]] = inp[i];
		++counter[c];
	}
}

template<class Type>
void LSDSort(Type* inp, int size, bool is64)
{
	Type* out = new Type[size];
	CountingSort(inp, out, 0, size);
	CountingSort(out, inp, 1, size);
	CountingSort(inp, out, 2, size);
	CountingSort(out, inp, 3, size);

	if (is64)
	{
		CountingSort(inp, out, 4, size);
		CountingSort(out, inp, 5, size);
		CountingSort(inp, out, 6, size);
		CountingSort(out, inp, 7, size);
	}
	delete[] out;
}

template<class Type>
int compare(const void* a, const void* b)
{
  return (*(Type *) a - *(Type *) b);
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	rand();
	bool debugMode = false;
	int size = 100000000; // 100 M
	int startTime, endTime, time32, time64;

	/* ------------------------ Radix Sort -------------------------- */

	__int32* testArray32 = new __int32[size];
	createTestData(testArray32, size);
	startTime = clock();
	LSDSort(testArray32, size, false);
	endTime = clock();
	time32 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray32, size);
	}
	delete [] testArray32;

	__int64* testArray64 = new __int64[size];
	createTestData(testArray64, size);
	startTime = clock();
	LSDSort(testArray64, size, true);
	endTime = clock();
	time64 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray64, size);
	}
	delete [] testArray64;

	std::cout << "Radix Sort:\n";
	printTime(time32, time64);

	/* --------------------------- STL ------------------------------ */
	
	__int32* testArray32Stl = new __int32[size];
	createTestData(testArray32Stl, size);
	startTime = clock();
	std::qsort(testArray32Stl, size, sizeof(__int32), compare<__int32>);
	endTime = clock();
	time32 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray32Stl, size);
	}
	delete [] testArray32Stl;

	__int64* testArray64Stl = new __int64[size];
	createTestData(testArray64Stl, size);
	startTime = clock();
	std::qsort(testArray64Stl, size, sizeof(__int64), compare<__int64>);
	endTime = clock();
	time64 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray64Stl, size);
	}
	
	std::cout << "\nStandart Quick Sort:\n";
	printTime(time32, time64);

	delete [] testArray64Stl;
	return 0;
}
