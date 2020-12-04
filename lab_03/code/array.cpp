#include "array.h"

arrayType createArray(int sizeArray)
{
    return new int[sizeArray];
}

void inputArray(arrayType& array, int sizeArray)
{
    std::cout << ">> Input elements by Enter:\n";

    for (int i = 0; i < sizeArray; i++) 
    {
        std::cin >> array[i];
    }
}

void outputArray(arrayType array, int size)
{
    std::cout << ">> Array: ";

    for (int i = 0; i < size; i++)
    {
        std::cout << array[i] << " ";
    }

    std::cout << "\n";
}

void deleteArray(arrayType array)
{
    delete [] array; 
}


// Generate array with random elements 
arrayType generateArray(int size)
{
    arrayType newArray = createArray(size);

    for (int i = 0; i < size; i++) 
    {
        newArray[i] = rand() % 100 + 0;
    }

    return newArray;
}