#include "Process.h"

// > Constructors:
Process::Process(){
    this->PID = -1;
    this->arrival_time = 0.0;
    this->total_service_time= 0.0;
    this->remaining_service_time = 0.0;
    this->static_priority = -1;
    this->total_execution_time = 0.0;
}

Process::Process(int PID, double arr_time, double svc_time, int priority){
    this->PID = PID;
    this->arrival_time = arr_time;
    this->total_service_time = svc_time;
    this->static_priority = priority;
    this->total_execution_time = 0.0;
}


// > Setters:
void Process::set_PID(int PID){
    this->PID = PID;
}

void Process::set_arr_time(double arr_time){
    this->arrival_time = arr_time;
}

void Process::set_svc_time(double svc_time){
    this->total_service_time = svc_time;
}

void Process::set_priority(int priority){
    this->static_priority = priority;
}

// > Getters:
int Process::get_PID(){
    return this->PID;
}

double Process::get_arr_time(){
    return this->arrival_time;
}

double Process::get_remaining_svc_time(){
    return this->remaining_service_time;
}

int Process::get_priority(){
    return this->static_priority;
}

double Process::get_total_execution_time(){
    return this->total_execution_time;
}

// > Misc:
void Process::do_execution(double amount){
    if (this->remaining_service_time - amount < 0){
        this->total_execution_time += remaining_service_time;
        remaining_service_time = 0.0;
    }

    this->total_execution_time += amount;
    this->remaining_service_time -= amount;
}