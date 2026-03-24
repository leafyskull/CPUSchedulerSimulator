#include "Process.h"

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
        Event(EventType eventType, Process* process);

        EventType get_eventType();
        Process* get_process();
};