#include "Process.h"

enum EventType {
    INVALID_TYPE,
    process_arrival,
    process_departure,
    quantum_expiration
};

class Event{
    private:
        EventType eventType;
        Process* process;
    public:
        Event();
        Event(EventType eventType, Process* process);

        EventType get_eventType();
        Process* get_process();
};