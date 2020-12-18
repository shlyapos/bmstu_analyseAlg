#include "conveyor.h"

// Public methods
Conveyor::Conveyor(int taskNumber, int taskLen) : ntask(taskNumber), taskLen(taskLen)
{
    ft1 = 0;
    ft2 = 0;
    ft3 = 0;
}

Conveyor::~Conveyor()
{

}



void Conveyor::run()
{
    generateRequest();

    std::thread t1 = std::thread(&Conveyor::part1, this);
    std::thread t2 = std::thread(&Conveyor::part2, this);
    std::thread t3 = std::thread(&Conveyor::part3, this);

    t1.join();
    t2.join();
    t3.join();
}


std::list<Request*> Conveyor::getResult()
{
    return result;
}


// Private methods
void Conveyor::generateRequest()
{
    for (int i = 0; i < ntask; i++)
    {
        Request *req = new Request(taskLen, i);
        req->generateString();

        startQ.push(req);
    }
}


void Conveyor::part1()
{
    for(; ft1 < ntask; ft1++)
    {
        Request *req;

        if (startQ.size())
        {
            req = startQ.front();
            startQ.pop();
        }
        else
            continue;

        req->timeS[0] = GetTime();
        req->cryptCaesar(12);
        req->timeE[0] = GetTime();

        m1.lock();
        q2.push(req);
        m1.unlock();
    }
}

void Conveyor::part2()
{
    while(q2.size() == 0)
        continue;

    for (; ft2 < ntask; ft2++)
    {
        while(q2.size() == 0)
            continue;

        Request *req;

        m1.lock();

        req = q2.front();
        q2.pop();

        m1.unlock();

        req->timeS[1] = GetTime();
        req->cryptXor('p');
        req->timeE[1] = GetTime();

        m2.lock();
        q3.push(req);
        m2.unlock();
    }
}

void Conveyor::part3()
{
    while(q3.size() == 0)
        continue;

    for(; ft3 < ntask; ft3++)
    {
        while (q3.size() == 0)
            continue;

        Request *req;

        m2.lock();

        req = q3.front();
        q3.pop();

        m2.unlock();

        req->timeS[2] = GetTime();
        req->cryptXor('a');
        req->timeE[2] = GetTime();

        result.push_back(req);
    }
}