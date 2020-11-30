#include "sort.h"

void static sortSwap(int& a, int& b)
{
    int buf = a;

    a = b;
    b = buf;
}

void sortBubble(arrayType& array, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j] > array[j + 1])
                sortSwap(array[j], array[j + 1]);
        }
    }
}

void sortInsertion(arrayType& array, int size)
{
    int key;
    int j;

    for (int i = 1; i < size; i++)
    {
        key = array[i];

        for (j = i - 1; j >= 0 && array[j] > key; j--)
            array[j + 1] = array[j];

        array[j + 1] = key;
    }
}

void sortShaker(arrayType& array, int size)
{
    int left = 1;
    int right = size - 1;

    while (left <= right)
    {
        for (int i = right; i >= left; i--)
        {
            if (array[i - 1] > array[i])
                sortSwap(array[i - 1], array[i]);
        }

        left++;

        for (int i = left; i <= right; i++)
        {
            if (array[i - 1] > array[i])
                sortSwap(array[i - 1], array[i]);
        }

        right--;
    }
}