#include "CPU.h"


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




