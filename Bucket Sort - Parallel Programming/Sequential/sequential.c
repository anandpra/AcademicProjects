#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include "ziggurat.h"
#include "rnd.h"
#include <sys/time.h>

int partition(float inputArray[], int left, int right);
void swap(float inputArray[], int i, int j);
float selectKthElement(float inputArray[], int left, int right, int i);
void quickSortBucket(float inputArray[], int blockStart, int blockEnd);

int size ;
int max = 10000000;

int main(int argc, char **argv){

	if(argc < 3){
		printf("Please provide problem size as '-t <Problem Size>'\n");
		return -1;
	}

	if(strcmp("-t",argv[1])==0){
		size = atoi(argv[2]);
	}
	/*Populate the array*/

	float* inputArray = (float *)malloc(size*sizeof(float));
	struct timeval begin,end;

	random_number_generator_normal(inputArray,size,max);

	int i;
	gettimeofday(&begin,NULL);

	float element;

	int blocks;
	int blockSize = size/10;
	int blockStart = 0;
	int blockEnd = blockStart + blockSize;

	for(blocks = 0 ; blocks < size/blockSize;blocks++){

		blockStart = blocks * blockSize;
		blockEnd = blockStart + blockSize - 1;

		element = selectKthElement(inputArray, blockStart, size - 1, blockSize);


		quickSortBucket(inputArray, blockStart, blockEnd);


	}

	gettimeofday(&end,NULL);

	double timeElapsed=(end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)/1000000.0;
	printf("\n%d \t %f\n",size,timeElapsed);

	return 0;
}

void quickSortBucket(float inputArray[], int left, int right){

	int index = partition(inputArray, left, right);

	if (left < index - 1)
		quickSortBucket(inputArray, left, index - 1);
	if (index < right)
		quickSortBucket(inputArray, index, right);
}

float selectKthElement(float inputArray[], int left, int right, int i){

	if( left == right)
		return inputArray[left];

	int randomIndex = rand() % (right - left) + left;
	//printf("\n RandomIndex:%d num:%f",randomIndex, inputArray[randomIndex]);

	swap(inputArray, randomIndex, right);

	int correctIndex = partition(inputArray, left, right);

	int k = correctIndex - left + 1;
	if( i == k)
		return inputArray[correctIndex];
	else if(i < k)
		return selectKthElement(inputArray, left, correctIndex - 1, i);
	else //if(i > k)
		return selectKthElement(inputArray,correctIndex + 1, right, i - k);
}

int partition(float inputArray[], int left, int right){

	if(left < 0 || right > size)
		return -1;

	float pivot = inputArray[right];
	int i = left - 1;
	int j;
	for( j = left; j < right; j++ ){

		if(inputArray[j] <= pivot){
			i++;
			swap(inputArray, i , j);
		}
	}

	swap(inputArray, i + 1, right);

	return i+1;
}

void swap(float inputArray[], int i, int j){
	float temp = inputArray[i];
	inputArray[i] = inputArray[j];
	inputArray[j] = temp;
}
