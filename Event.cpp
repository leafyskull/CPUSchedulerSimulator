#include "Event.h"


// Overload of < operator for keeping priority queue in order
bool operator<(Event& lhs, Event& rhs){
    return (lhs.getTime() < rhs.getTime());
}


// > Constructors:
Event::Event(){
    this->eventType = INVALID_TYPE;
    this->process = nullptr;
    this->time = 0.0;
}

Event::Event(EventType eventType, Process* proc, double time){
    this->eventType = eventType;
    this->process = proc;
    this->time = time;
}

// > Setters:
void Event::setTime(double time){
    this->time = time;
}

// > Getters:
EventType Event::get_eventType(){
    return this->eventType;
}

Process* Event::get_process(){
    return this->process;
}

double Event::getTime(){
    return this->time;
}