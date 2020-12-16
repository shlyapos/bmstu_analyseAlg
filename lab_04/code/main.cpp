#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <windows.h>

using namespace std;
using matrixType = int**;

std::mutex mtx;


// Matrix methods
matrixType createMatrix(int n, int m)
{
    matrixType ptr = new int*[n];

    for (int i = 0; i < n; i++)
    {
        ptr[i] = new int[m];
    }

    return ptr;
}

void inputMatrix(matrixType& matrix, int n, int m)
{
    std::cout << ">> Input elements by Enter:\n";

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < m; j++)
        {
            cin >> matrix[i][j];
        }
        cout << "\n";
    }
}

void outputMatrix(matrixType matrix, int n, int m)
{
    cout << ">> Matrix: \n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << matrix[i][j] << '\t';
        }
        cout << "\n";
    }

    cout << "\n";
}

void deleteMatrix(matrixType matrix, int n)
{
    for (int i = 0; i < n; i++)
        delete [] matrix[i];

    delete [] matrix;
}

matrixType generateMatrix(int n, int m)
{
    matrixType matrix = createMatrix(n, m);

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    return matrix;
}


// Classic multiplication
matrixType classicMultiplication(matrixType a, matrixType b, int m, int n, int q)
{
    matrixType c = createMatrix(m, q);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            int res = 0;

            for (int k = 0; k < n; k++)
            {
                res += a[i][k] + b[k][j];
            }

            c[i][j] = res;
        }
    }

    return c;
}


// Parallel multiplication for row
void multThreadRow(matrixType& c, matrixType& a, matrixType&b, int m, int n, int q, int start, int step)
{
    for (int i = start; i < m; i += step)
    {
        for (int j = 0; j < q; j++)
        {
            int res = 0;

            for (int k = 0; k < n; k++)
            {
                res += a[i][k] * b[k][j];
            }
            mtx.lock();
            c[i][j] = res;
            mtx.unlock();
        }
    }
}

matrixType multiplicationParallelRow(matrixType a, matrixType b, int m, int n, int q, int threadCnt)
{
    matrixType c = createMatrix(m, q);
    vector<thread> threadArray;

    for (int i = 0; i < threadCnt; i++)
    {
        threadArray.push_back(thread(multThreadRow, ref(c), ref(a), ref(b), m, n, q, i, threadCnt));
    }

    for (int i = 0; i < threadCnt; i++)
    {
        threadArray[i].join();
    }

    return c;
}


// Parallel multiplication for column
void multThreadColumn(matrixType& c, matrixType& a, matrixType& b, int m, int n, int q, int start, int step)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = start; j < q; j += step)
        {
            int res = 0;

            for (int k = 0; k < n; k++)
            {
                res += a[i][k] * b[k][j];
            }

            mtx.lock();
            c[i][j] = res;
            mtx.unlock();
        }
    }
}

matrixType multiplicationParallelColumn(matrixType a, matrixType b, int m, int n, int q, int threadCnt)
{
    matrixType c = createMatrix(m, q);
    vector<thread> threadArray;

    for (int i = 0; i < threadCnt; i++)
    {
        threadArray.push_back(thread(multThreadColumn, ref(c), ref(a), ref(b), m, n, q, i, threadCnt));
    }

    for (int i = 0; i < threadCnt; i++)
    {
        threadArray[i].join();
    }

    return c;
}


// Time test
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);///1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}


void beginTimeTest()
{
    int countSize = 5;
    int sizes[] = { 32, 100, 250, 500, 1000 };

    char algorithmNames[][100] = {"Classic multiplication", "Parallel by row", "Parallel by column"};

    //for (int k = 0; )
    for (int i = 0; i < countSize; i++)
    {
        matrixType a = generateMatrix(sizes[i], sizes[i]);
        matrixType b = generateMatrix(sizes[i], sizes[i]);
        
        double finishTime = 0;

        for (int j = 0; j < 5; j++)
        {
            StartCounter();
            classicMultiplication(a, b, sizes[i], sizes[i], sizes[i]);
            finishTime += GetCounter();
        }

        finishTime /= 5;

        cout << "\nFor " << algorithmNames[0] << "\t-> Row - " << sizes[i] 
                                              << "\tColumn - " << sizes[i]
                                              << "\tTime - " << finishTime;
        deleteMatrix(a, sizes[i]);
        deleteMatrix(b, sizes[i]);
    }

    matrixType (*algorithms[])(matrixType, matrixType, int, int, int, int) = {multiplicationParallelRow, 
                                                                              multiplicationParallelColumn};
    /*Индекс отвечает за смену имени алгоритмов*/
    int p = 1;

    int nThreads[] = { 1, 2, 4, 8, 16, 32 };

    for (int k = 0; k < 2; k++)
    {
        matrixType (*mult)(matrixType, matrixType, int, int, int, int) = algorithms[k];

        for (int i = 0; i < countSize; i++)
        {
            for (int threads = 0; threads < 6; threads++)
            {
                matrixType a = generateMatrix(sizes[i], sizes[i]);
                matrixType b = generateMatrix(sizes[i], sizes[i]);

                double finishTime = 0;

                for (int j = 0; j < 5; j++)
                {
                    StartCounter();
                    mult(a, b, sizes[i], sizes[i], sizes[i], nThreads[threads]);
                    finishTime += GetCounter();
                }

                finishTime /= 5;

                cout << "\nFor " << algorithmNames[p] << "\t-> Row - " << sizes[i] 
                                              << "\tColumn - " << sizes[i]
                                              << "\tTime - " << finishTime
                                              << "\tTreads - " << nThreads[threads];

                /*Индекс отвечает за смену имени алгоритмов*/
                p++;

                deleteMatrix(a, sizes[i]);
                deleteMatrix(b, sizes[i]);
            }
        }
    }
}


int main()
{
    beginTimeTest();

    int n, m, n1, q;

    printf("Input row number for A:");
    cin >> n;

    printf("Input column number for A: ");
    cin >> m;

    printf("Input row number for B: ");
    cin >> n1;

    printf("Input column number for B: ");
    cin >> q;

    matrixType a = createMatrix(n, m);
    matrixType b = createMatrix(n1, q);

    inputMatrix(a, n, m);
    inputMatrix(b, n1, q);

    matrixType c = multiplicationParallelColumn(a, b, n, m, q, 3);

    outputMatrix(c, n, q); 

    deleteMatrix(a, n);
    deleteMatrix(b, q);
    deleteMatrix(c, q);
}