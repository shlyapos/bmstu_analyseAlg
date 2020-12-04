#include "mainHead.h"

// Out menu and take user choice 
int outMenu()
{
    int choice;

    std::cout << "\n>> Choose method for sort:";
    std::cout <<   "\n>> [1] - Bubble sort";
    std::cout <<   "\n>> [2] - Insertion sort";
    std::cout <<   "\n>> [3] - Shaker sort";
    std::cout <<   "\n>> [4] - Time test";
    std::cout << "\n\n>> [0] - Exit";

    std::cout << "\n\n>> You're choice: ";
    std::cin >> choice;

    return choice;
}

// Do time tests for sort functions
void beginTimeTest()
{
    int countSizes = 5;
    int sizes[] = {100, 200, 300, 1000, 10000};
    int count[] = {100, 100, 100, 10, 1};

    void (*algorithms[])(arrayType&, int) = {sortBubble, sortInsertion, sortShaker}; 

    char algorithmNames[][100] = {"Bubble sort", "Insert sort", "Shaker sort"};

    for (int k = 0; k < 3; k++)
    {
        void (*sort)(arrayType&, int) = algorithms[k];

        for (int i = 0; i < countSizes; i++)
        {
            arrayType testArray = generateArray(sizes[i]);
            
            double finishTime = 0;

            for (int j = 0; j < count[i]; j++)
            {
                StartCounter();
                sort(testArray, sizes[i]);
                finishTime += GetCounter();
            }

            finishTime /= count[i];
            
            std::cout << "\nFor " << algorithmNames[k] << "\t-> Size - " << sizes[i] << "\tTime - " << finishTime;

            deleteArray(testArray);
        }

        std::cout << "\n---------------------";
    }
}


int main(void)
{
    int done = false;
    int choice;

    arrayType ptrArray;
    int sizeArray;

    while (!done)
    {
        choice = outMenu();

        if (choice == EXIT)
        {
            done = true;
            std::cout << "\n\n>>> Exit program...";
        }
        else if (choice == TIME_TEST)
        {
            beginTimeTest();
        }
        else 
        {
            std::cout << "\n\n>> Input array length: ";
            std::cin >> sizeArray;

            ptrArray = createArray(sizeArray);

            inputArray(ptrArray, sizeArray);
            
            switch (choice)
            {
                case BUBBLE_SORT:
                    sortBubble(ptrArray, sizeArray);
                    break;
                case INSERT_SORT:
                    sortInsertion(ptrArray, sizeArray);
                    break;
                case SHAKER_SORT:
                    sortShaker(ptrArray, sizeArray);
                    break;
                default:
                    std::cout << ">>> Wrong choice, try again.";
                    break;
            }

            if (choice < 4)
            {
                outputArray(ptrArray, sizeArray); 
            }

            deleteArray(ptrArray);
        }
    }
}