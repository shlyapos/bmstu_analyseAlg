#include <iostream>
#include <random>

using arrayType = int*;

arrayType createArray(int sizeArray);
void inputArray(arrayType& array, int sizeArray);
void outputArray(arrayType array, int size);
void deleteArray(arrayType array);

arrayType generateArray(int size);