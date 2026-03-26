#ifndef Event_H
#define Event_H

#include "Process.h"
#include <string>
using namespace std;

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
        bool operator<(const Event& other) const;

        Event();
        Event(EventType eventType, Process* process, double time);

        EventType get_eventType();
        string get_eventType_as_string();
        Process* get_process();

        
        void setTime(double time);
        double getTime() const;
};

#endif