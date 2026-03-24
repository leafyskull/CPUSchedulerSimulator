#include "Event.h"


// Constructor
Event::Event(EventType eventType, Process* proc){
    this->eventType = eventType;
    this->process = proc;
}

// > Getters:
EventType Event::get_eventType(){
    return this->eventType;
}

Process* Event::get_process(){
    return this->process;
}