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
#include <cstdint>

template<class Type>
void createTestData(Type* &data, const int &size)
{
	srand(time(nullptr));
	rand();
	for (int i = 0; i < size; i++)
	{
		data[i] = rand() * rand() % INT64_MAX;
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
	unsigned char* arrayOfBytes = reinterpret_cast<unsigned char *>(inp);
	int counter[256];
	memset(counter, 0, sizeof counter);
	for (int i = 0; i < size; i++)
	{
		++counter[arrayOfBytes[sizeof(Type) * i + byteNum]];
	}
	int j = 0;
	for (; j < 256; j++)
	{
		if (counter[j] != 0)
			break;
	}
	int currentValueOfCounter = counter[j];
	counter[j] = 0;
	j++;
	for (; j < 256; j++)
	{
		int b = counter[j];
		counter[j] = currentValueOfCounter;
		currentValueOfCounter += b;
	}
	unsigned char c;
	for (int i = 0; i < size; i++)
	{
		c = arrayOfBytes[sizeof(Type) * i + byteNum];
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
void CountingSort2(Type* inp, Type* out, int maskNumber, int size)
{
	int counter[65536];
	memset(counter, 0, sizeof counter);
	unsigned __int64 offset = maskNumber << 4;
	for (int i = 0; i < size; i++)
	{
		++counter[(inp[i] & (65535 << offset)) >> offset];
	}

	int j = 0;
	for (; j < 65536; j++)
	{
		if (counter[j] != 0)
			break;
	}

	int currentValueOfCounter = counter[j];
	counter[j] = 0;
	j++;
	for (; j < 65536; j++)
	{
		int b = counter[j];
		counter[j] = currentValueOfCounter;
		currentValueOfCounter += b;
	}

	unsigned __int64 c;
	for (int i = 0; i < size; i++)
	{
		c = (inp[i] & (65535 << offset)) >> offset;
		out[counter[c]] = inp[i];
		++counter[c];
	}
}

template<class Type>
void CountingSort3(Type* inp, Type* out, int maskNumber, int size)
{
	int counter[256];
	memset(counter, 0, sizeof counter);
	unsigned __int64 offset = maskNumber << 3;
	for (int i = 0; i < size; i++)
	{
		++counter[(inp[i] & (255 << offset)) >> offset];
	}

	int j = 0;
	for (; j < 256; j++)
	{
		if (counter[j] != 0)
			break;
	}

	int currentValueOfCounter = counter[j];
	counter[j] = 0;
	j++;
	for (; j < 256; j++)
	{
		int b = counter[j];
		counter[j] = currentValueOfCounter;
		currentValueOfCounter += b;
	}

	unsigned char c;
	for (int i = 0; i < size; i++)
	{
		c = (inp[i] & (255 << offset)) >> offset;
		out[counter[c]] = inp[i];
		++counter[c];
	}
}

template<class Type>
void LSDSort2(Type* inp, int size, bool is64)
{
	Type* out = new Type[size];
	CountingSort2(inp, out, 0, size);
	CountingSort2(out, inp, 1, size);

	if (is64)
	{
		CountingSort2(inp, out, 2, size);
		CountingSort2(out, inp, 3, size);
	}
	delete[] out;
}

template<class Type>
void LSDSort3(Type* inp, int size, bool is64)
{
	Type* out = new Type[size];
	CountingSort3(inp, out, 0, size);
	CountingSort3(out, inp, 1, size);
	CountingSort3(inp, out, 2, size);
	CountingSort3(out, inp, 3, size);

	if (is64)
	{
		CountingSort3(inp, out, 4, size);
		CountingSort3(out, inp, 5, size);
		CountingSort3(inp, out, 6, size);
		CountingSort3(out, inp, 7, size);
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
	bool debugMode = false;
	const int size = 10000000; // 100 M
	int startTime, endTime, time32, time64, time32_2, time64_2;

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
	
	/* ------------------------ Radix Sort 2 ------------------------- */

	__int32* testArray32_2 = new __int32[size];
	createTestData(testArray32_2, size);
	startTime = clock();
	LSDSort2(testArray32_2, size, false);
	endTime = clock();
	time32_2 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray32_2, size);
	}
	delete [] testArray32_2;

	__int64* testArray64_2 = new __int64[size];
	createTestData(testArray64_2, size);
	startTime = clock();
	LSDSort2(testArray64_2, size, true);
	endTime = clock();
	time64_2 = endTime - startTime;
	if (debugMode)
	{
		printResult(testArray64_2, size);
	}
	delete [] testArray64_2;

	std::cout << "Radix Sort 2:\n";
	printTime(time32_2, time64_2);

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
