#include "timer.h"

double GetTime()
{
    LARGE_INTEGER li;
    !QueryPerformanceFrequency(&li);
    double PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    return double(li.QuadPart) / PCFreq * 1000;
}