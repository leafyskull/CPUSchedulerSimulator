#include <iostream>
#include "Process.cpp"
using namespace std;



int main(){

    double average_arrival_rate;
    double average_service_rate;
    double base_quantum;
    double scaling_factor_A;
    double scaling_factor_B;

    cin >> average_arrival_rate >> average_service_rate >>
           base_quantum >> scaling_factor_A >> scaling_factor_B;






    return 0;
}


int calculate_quantum(double base_quantum, double A, double B, Process proc){

    double return_quantum = base_quantum + ((10 - proc.get_priority()) / A) - (B * proc.get_total_execution_time());

    if (return_quantum < 0) return_quantum = 0.01;

    return return_quantum;
}