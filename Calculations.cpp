#include <random>
#include <iostream>
#include "Calculations.h"



Calculations::Calculations(){}

// Calculates an arrival time based off a poisson distribution,
// given an average arrival rate per second.
double Calculations::calculate_arrival_time(int average_arrival_rate){

    std::random_device rand;
    std::mt19937 gen(rand());

    std::exponential_distribution<double> dist(average_arrival_rate);

    return dist(gen);
};

// Calculates a process' service time based off an exponential
// distribution, given an average service time.
double Calculations::calculate_service_time(double average_service_time){

    std::random_device rand;
    std::mt19937 gen(rand());

    double lambda = 1.0 / average_service_time;

    std::exponential_distribution<double> dist(lambda);

    return dist(gen);
};

double Calculations::calculate_quantum(double base_quantum, double A, double B, Process* proc){

    double return_quantum = base_quantum + ((10.0 - (*proc).get_priority()) / A) - (B * (*proc).get_total_execution_time());

    if (return_quantum < 0.01) return_quantum = 0.01;

    return return_quantum;
};

int Calculations::get_random_static_priority(){

    std::random_device rand;
    std::mt19937 gen(rand());
    
    std::uniform_int_distribution<> dist(1, 10);

    return dist(gen);
};



