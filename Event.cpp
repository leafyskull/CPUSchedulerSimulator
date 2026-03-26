#include "Event.h"
#include <string>
using namespace std;


// Overload of < operator for keeping priority queue in order
bool Event::operator<(const Event& other) const{
    return (this->time > other.getTime());
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

string Event::get_eventType_as_string(){
    int enumIndex = this->eventType;
    string enumAsString;

    switch (enumIndex){
        case 0: // INVALID
            enumAsString = "INVALID";
            break;
        case 1:
            enumAsString = "Process arrival";
            break;
        case 2:
            enumAsString = "Process departure";
            break;
        case 3:
            enumAsString = "Service arrival";
            break;
        case 4:
            enumAsString = "Quantum expiration";
            break;
    }

    return enumAsString;
}

Process* Event::get_process(){
    return this->process;
}

double Event::getTime() const{
    return this->time;
}