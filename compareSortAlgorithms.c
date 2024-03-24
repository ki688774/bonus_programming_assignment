#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void intSwap(int *a, int *b) {
	int tempInt = *a;
	*a = *b;
	*b = tempInt;
}

void percolateDown(int arr[], int n, int targetIndex) {
	//Abort the mission if the target index is a leaf or the target index is out of bounds.
	if ((targetIndex + 1) * 2 > n) {
		return;
	}

	//Find the smaller (or only) child of the target value.
	int swapIndex;

	if ((targetIndex + 1) * 2 < n && arr[(targetIndex + 1) * 2] < arr[((targetIndex + 1) * 2) - 1]) {
		swapIndex = (targetIndex + 1) * 2;
	} else {
		swapIndex = ((targetIndex + 1) * 2) - 1;
	}

	//Check if the target value is bigger than its smaller (or only) child. If so, percolate.
	if (arr[targetIndex] > arr[swapIndex]) {
		intSwap((arr + targetIndex), (arr + swapIndex));
		percolateDown(arr, n, swapIndex);
	}

}

//This doesn't automatically reduce n. You'll have to do that yourself.
int popHeap(int arr[], int n) {
	if (n < 1) {
		return 0;
	}

	intSwap(arr, (arr + n - 1));
	percolateDown(arr, (n - 1), 0);
	return arr[n - 1];
}

//You should only be calling heapify with headIndex as 0. Anything else is for heapify recursion only.
void heapify(int arr[], int n, int headIndex) {
	//Abort the mission if the head index is a leaf or the target index is out of bounds.
	if ((headIndex + 1) * 2 > n) {
		return;
	}

	//Call heapify on the children of headIndex (whether or not they exist).
	heapify(arr, n, ((headIndex + 1) * 2) - 1);
	heapify(arr, n, (headIndex + 1) * 2);

	//Then, percolate down the head.
	percolateDown(arr, n, headIndex);
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	//First, heapify the array.
	heapify(arr, n, 0);

	//Then, copy the heap.
	int *copyArray = Alloc(sizeof(int) * n);
	for (int c = 0; c < n; c++) {
		copyArray[c] = arr[c];
	}

	//Finally, pop the entire copied heap back into the original array.
	for (int c = 0; c < n; c++) {
		arr[c] = popHeap(copyArray, (n - c));
	}

	DeAlloc(copyArray);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//If the array is 1 or smaller, do nothing.
	//It should never be smaller, but you never know when some maniac will try to break reality.
	if (l >= r) {
		return;
	}

	//Find the middle of the array.
	int m = (l + r)/2;

	//Run a merge sort on the each individual half of the array.
	mergeSort(pData, l, m);
	mergeSort(pData, (m + 1), r);

	//Then, splice the two halves together.
	int leftIndex = l;
	int rightIndex = (m + 1);
	int *tempArray = Alloc(sizeof(int) * (r + 1 - l));
	int tempArrayIndex = 0;

	while (leftIndex <= m && rightIndex <= r) {
		if (pData[leftIndex] < pData[rightIndex]) {
			tempArray[tempArrayIndex] = pData[leftIndex];
			leftIndex++;
		} else {
			tempArray[tempArrayIndex] = pData[rightIndex];
			rightIndex++;
		}
		tempArrayIndex++;
	}

	//While statements to dump the rest of the remaining half into tempArray.
	while (leftIndex <= m) {
		tempArray[tempArrayIndex] = pData[leftIndex];
		leftIndex++;
		tempArrayIndex++;
	}

	while (rightIndex <= r) {
		tempArray[tempArrayIndex] = pData[rightIndex];
		rightIndex++;
		tempArrayIndex++;
	}

	//Final part of splicing: put everything from tempArray to its rightful place in pData.
	for (int c = 0; c < tempArrayIndex; c++) {
		pData[l + c] = tempArray[c];
	}

	DeAlloc(tempArray);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	for (int activeSlots = 2; activeSlots <= n; (activeSlots)++) {
		for (int c = activeSlots - 1; c > 0; c--) {
			if (pData[c] < pData[c - 1]) {
				intSwap((pData + c), (pData + c - 1));
			} else {
				break;
			}
		}
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for (int unsortedSlots = n; unsortedSlots > 1; unsortedSlots--) {
		for (int c = 0; c < (unsortedSlots - 1); c++) {
			if (pData[c] > pData[c + 1]) {
				intSwap((pData + c), (pData + c + 1));
			}
		}
	}

}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for (int sortedSlots = 0; sortedSlots < n - 1; sortedSlots++) {
		int tempIndex = sortedSlots;
		for (int c = sortedSlots + 1; c < n; c++) {
			if (pData[c] < pData[tempIndex]) {
				tempIndex = c;
			}
		}

		//Figured that a comparison would probably cost less performance than a swap.
		if (tempIndex != sortedSlots) {
			intSwap((pData + tempIndex), (pData + sortedSlots));
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		for (int c = 0; c < dataSz; c++) {
			fscanf(inFile, "%d ", *ppData + c);
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		//Despite heapSort only having two arguments, the original line of code feeds in three.
		//I replaced the line with what it (probably) should be, but the original is still here (just commented out).
		//heapSort(pDataCopy, 0, dataSz - 1);
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
