#include "Process.h"

class Calculations{

    public:
        double calculate_arrival_time(int average_arrival_rate);
        double calculate_service_time(double average_service_time);
        double calculate_quantum(double base_quantum, double A, double B, Process* proc);
        int get_random_static_priority();
};