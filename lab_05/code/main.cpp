#include "conveyor.h"

#include <iostream>
#include <fstream>

int main(void)
{
    int nconveyors = 20;
    int nrequest = 200;

    Conveyor *conveyors[20];
    double times[200] { 0 };

    for (int i = 0; i < nconveyors; i++)
    {
        conveyors[i] = new Conveyor(nrequest, 1000000);
        conveyors[i]->run();
    }

    std::list<Request*> results[20];

    for (int i = 0; i < nconveyors; i++)
        results[i] = conveyors[i]->getResult();

    int c;

    // Output time for second part
    std::cout << ">>> Second part time:\n";
    for (int i = 0; i < nconveyors; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->timeInSeqondQ();
            c++;
        }
    }
    for (int i = 0; i < nrequest; i++)
    {
        std::cout << i << " " << times[i] / nconveyors << '\n';
        times[i] = 0;
    }


    // Output time for third part 
    std::cout << "\n\n>>> Third part time:\n";
    for (int i = 0; i < nconveyors; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->timeInThirdQ();
            c++;
        }
    }
    for (int i = 0; i < nrequest; i++)
    {
        std::cout << i << " " << times[i] / nconveyors << '\n';
        times[i] = 0;
    }


    // Output summary time
    std::cout << "\n\n>>> Summary time:\n";
    for (int i = 0; i < nconveyors; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->summaryTimeQ();
            c++;
        }
    }
    for (int i = 0; i < nrequest; i++)
    {
        std::cout << i << " " << times[i] / nconveyors << '\n';
    }

    for (int i = 0; i < nconveyors; i++)
        delete conveyors[i];
}