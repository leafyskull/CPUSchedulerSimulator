#ifndef CPU_H
#define CPU_H

#include "Process.h"
#include <queue>

using namespace std;

class CPU{
    private:
        bool busy;
        queue<Process*> readyQueue;
    public:
        CPU();
        bool isBusy();
        void setBusy();
        void setIdle();

        void readyQueuePush(Process* proc);
        void readyQueuePop();
        Process* readyQueueFront();
        bool readyQueueIsEmpty();
        int readyQueueSize();
};

#endif