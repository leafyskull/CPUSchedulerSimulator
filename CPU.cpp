#include "CPU.h"


CPU::CPU(){
    this->busy = false;
}

void CPU::setBusy(){
    this->busy = true;
}

void CPU::setIdle(){
    this->busy = false;
}




