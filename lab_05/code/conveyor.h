#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <list>

#include "timer.h"

struct Request
{
public:
    Request(int lenght, int num) : len(lenght), num(num) {    }
    ~Request() {    }

    void generateString()
    {
        srand(time(0));

        for (int i = 0; i < len; i++)
        {
            dataStr.push_back(rand() % 26 + 97);
        }
    }

    void cryptXor(char k)
    {
        for (int i = 0; i < len; i++)
        {
            dataStr[i] ^= k;
        }
    }
    void cryptCaesar(int shift)
    {
        for (int i = 0; i < len; i++)
        {
            dataStr[i] = (dataStr[i] - 97 + shift) % 26 + 97;
        }
    }

    double timeInSeqondQ() { return timeS[1] - timeE[0]; }
    double timeInThirdQ()  { return timeS[2] - timeE[1]; }
    double summaryTimeQ()  { return timeE[2] - timeS[0]; }
    
    double timeS[3];
    double timeE[3];

    std::string dataStr;
    int len, num;
};


class Conveyor
{
public:
    Conveyor(int, int);
    ~Conveyor();
    
    void run();
    std::list<Request*> getResult();

private:
    void generateRequest();
    
    void part1();
    void part2();
    void part3();

    std::queue<Request*> startQ, q2, q3;
    std::list<Request*> result;

    std::mutex m1, m2;

    int ntask, taskLen;
    int ft1, ft2, ft3;
};

#endif // CONVEYOR_H