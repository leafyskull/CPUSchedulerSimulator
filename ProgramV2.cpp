#include <iostream>
#include <random>
#include <queue>
#include "Event.h"
#include "Process.h"
#include "Calculations.h"
#include "CPU.h"
#include "Calculations.h"
using namespace std;

void Simulator(double avg_arr_rate, double avg_svc_rate, double base_quant, double A, double B);

int main(){

    double average_arrival_rate;
    double average_service_rate;
    double base_quantum;
    double scaling_factor_A;
    double scaling_factor_B;

    cin >> average_arrival_rate >> average_service_rate >>
           base_quantum >> scaling_factor_A >> scaling_factor_B;

    
    Simulator(average_arrival_rate, average_service_rate, base_quantum, 
              scaling_factor_A, scaling_factor_B);



    return 0;
}

void Simulator(double avg_arr_rate, double avg_svc_rate, double base_quant, double A, double B){
    
    // Need to track:
    // - Average turnaround time
    //   Have each process track turnaround time, and average at end?
    // - Average number of context switches
    //   Have each process track their number of arrivals?
    // - Average number of processes in the ready queue
    //   ?


    Calculations calculator;

    queue<Process> readyQueue; // Ready queue of processes
    priority_queue<Event> eventQueue; // Queue of events
    CPU cpu; // CPU we will use

    double currentTime = 0.0; // Total time clock
    double busyTime = 0.0; // Total time CPU spent executing
    double generatedTime = 0.0; // For generating the process arrival times

    

    // 1. Generate a workload of 10,000 processes
    const int NUM_PROCESSES = 10; // 10 for testing
    int completedProcesses = 0;
    Process processes[NUM_PROCESSES];

    double service_time, local_arrival_time;
    int priority;

    cout << "*** Generating processes ***" << endl;
    for (int i = 0; i < NUM_PROCESSES; i++){
        service_time = calculator.calculate_service_time(avg_svc_rate);
        local_arrival_time = calculator.calculate_arrival_time(avg_arr_rate);
        priority = calculator.get_random_static_priority();

        generatedTime += local_arrival_time;
        
        processes[i].set_PID(i);
        processes[i].set_svc_time(service_time);
        processes[i].set_arr_time(generatedTime);
        processes[i].set_priority(priority);

        Event arrivalEvent = Event(process_arrival, &(processes[i]), generatedTime);
        eventQueue.push(arrivalEvent);

        cout << "******************************" << endl;
        cout << "Process generated!" << endl;
        cout << "PID: " << processes[i].get_PID() << endl;
        cout << "Svc time: " << processes[i].get_remaining_svc_time() << endl;
        cout << "Arr time: " << processes[i].get_arr_time() << endl;
        cout << "Static priority: " << processes[i].get_priority() << endl;
        cout << "******************************" << endl << endl;;
    }

    // **********************************************************************
    // CURRENT LOOP IDEA:
    //
    // Grab event
    //
    // Arrival:
    // - If CPU is free, schedule service event for right now.
    // - Else, add to readyQueue
    // Departure: 
    // - Set CPU to idle, "finish" process
    // Service Arrival:
    // - Set CPU to busy
    // - Calculate and "execute" quantum on process
    // - Schedule quantum_expiration process (which should happen 
    //   directly after)
    //   Quantum expiration:
    // - Determine if process is finished or not.
    //   - If so: create departure event
    //   - If not: back into readyQueue
    // - Scheudule service arrival of next process in ready queue
    //
    // **********************************************************************

    // MAIN LOOP
    while (completedProcesses < NUM_PROCESSES){

        // Grab next event in eventQueue
        Event eventToExecute = eventQueue.top();
        eventQueue.pop();

        Process* currentProcess = eventToExecute.get_process();

        currentTime = eventToExecute.getTime();


        // May now be redundant:
        //
        // // If we have idle time until the next process' arrival - fast forward to arrival
        // if (!cpu.isBusy() && currentTime < (*currentProcess).get_arr_time() 
        //     && eventToExecute.get_eventType() == process_arrival){
        //     currentTime = (*currentProcess).get_arr_time();
        // }



        if (eventToExecute.get_eventType() == process_arrival){

            // If CPU is free, schedule service event now.
            if (!cpu.isBusy()){
                Event serviceEvent = Event(service_arrival, currentProcess, currentTime);
            } else { // Else, add to readyQueue.
                readyQueue.push(*currentProcess);
            }

        } else if (eventToExecute.get_eventType() == process_departure){

            cpu.setIdle();
            completedProcesses++;

        } else if (eventToExecute.get_eventType() == service_arrival){

            cpu.setBusy();
            
            // Calculate quantum and do execution
            double quantum = calculator.calculate_quantum(base_quant, A, B, currentProcess);
            currentProcess->do_execution(quantum);

            // Schedule quantum_expiration event
            Event quantumExpEvent = Event(quantum_expiration, currentProcess, currentTime + quantum);

        } else if (eventToExecute.get_eventType() == quantum_expiration){

            // Figure out if process needs more work or is done.
            if ((*currentProcess).get_remaining_svc_time() > 0.0){
                readyQueue.push(*currentProcess);
            } else {
                Event departureEvent = Event(process_departure, currentProcess, currentTime);
                eventQueue.push(departureEvent);
            }

            cpu.setIdle();

            // Schedule service arrival of next process in readyQueue
            Process* nextProcess = &(readyQueue.front());
            Event nextProcessServiceArrival = Event(service_arrival, nextProcess, currentTime);

        } else { // ERROR: Invalid event type
            cout << "\n******************************" << endl;;
            cout << "ERROR: Invalid event type!" << endl;;
            cout << "Event type: " << eventToExecute.get_eventType() << endl;
            cout << "******************************\n" << endl;;
        }



    }
    


}

