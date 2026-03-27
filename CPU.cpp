#include "CPU.h"
#include "Process.h"
#include <queue>


CPU::CPU(){
    this->busy = false;
}

bool CPU::isBusy(){
    return this->busy;
}

void CPU::setBusy(){
    this->busy = true;
}

void CPU::setIdle(){
    this->busy = false;
}

void CPU::readyQueuePush(Process* proc){
    this->readyQueue.push(proc);
}

void CPU::readyQueuePop(){
    this->readyQueue.pop();
}

Process* CPU::readyQueueFront(){
    return this->readyQueue.front();
}

bool CPU::readyQueueIsEmpty(){
    return this->readyQueue.empty();
}

int CPU::readyQueueSize(){
    return this->readyQueue.size();
}




