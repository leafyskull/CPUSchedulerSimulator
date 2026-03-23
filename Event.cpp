#include "Process.cpp"

enum EventType {
    process_arrival,
    process_departure,
    quantum_expiration
};

class Event{

    private:
        EventType eventType;
        Process* process;
    public:
        EventType get_eventType();
        Process* get_process();

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
};