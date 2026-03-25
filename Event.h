#ifndef Event_H
#define Event_H

#include "Process.h"

enum EventType {
    INVALID_TYPE,
    process_arrival,
    process_departure,
    service_arrival,
    quantum_expiration
};

class Event{
    private:
        EventType eventType;
        Process* process;
        double time;
    public:
        Event();
        Event(EventType eventType, Process* process, double time);

        EventType get_eventType();
        Process* get_process();

        
        void setTime(double time);
        double getTime();
};

#endif