#include <iostream>
#include <random>
#include <queue>
#include "Event.h"
#include "Process.h"
#include "Calculations.h"
#include "CPU.h"
#include "Calculations.h"
using namespace std;



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
    queue<Event> eventQueue; // Queue of events
    CPU cpu; // CPU we will use

    double currentTime = 0.0; // Total time clock
    double busyTime = 0.0; // Total time CPU spent executing
    double generatedTime = 0.0; // For generating the process arrival times

    

    // 1. Generate a workload of 10,000 processes
    const int NUM_PROCESSES = 10000;
    int completedProcesses = 0;
    Process processes[NUM_PROCESSES];

    double service_time, local_arrival_time;
    int priority;

    for (int i = 0; i < NUM_PROCESSES; i++){
        service_time = calculator.calculate_service_time(avg_svc_rate);
        local_arrival_time = calculator.calculate_arrival_time(avg_arr_rate);
        priority = calculator.get_random_static_priority();
        
        processes[i].set_PID(i);
        processes[i].set_svc_time(service_time);
        processes[i].set_arr_time(generatedTime + local_arrival_time);
        processes[i].set_priority(priority);

        generatedTime += processes[i].get_remaining_svc_time();

        Event arrivalEvent = Event(process_arrival, &(processes[i]));
        eventQueue.push(arrivalEvent);
    }

    // **************************************************
    // 2. Main idea rough draft:

    //    - CPU:
    //    - Grab first event - arrival
    //    - 1. Perform quantum of "executing"
    //    - 2. Create departure event
    //    - Handle process departure, put back in ready queue.

    //    - When a process arrives:
    //    - 1. Create arrival event
    //    - 2. Put in ready queue


    // **************************************************

    // IDEA:
    // *For now, let's assume that we're doing FIFO*

    // Start clock
    // Create first ready event

    // Loop:
    // Go to next arrival event, set clock to this time
    // Create arrival for the following process
    // Get process that corresponds to ready event:
    // - Create departure event
    // - Handle quantum
    // - Handle departure, place back in ready queue
    // Update clock based on elapsed quantum
    // Grab next arrival event, continue
    // **************************************************

    // Get first process, create ready event:
    Process* firstProcess = &processes[0];
    Event firstProcessReadyEvent = Event(process_arrival, firstProcess);
    eventQueue.push(firstProcessReadyEvent);

    // MAIN LOOP
    while (completedProcesses < NUM_PROCESSES){

        // Grab next event in eventQueue
        Event eventToExecute = eventQueue.front();
        Process* currentProcess = eventToExecute.get_process();
        eventQueue.pop();

        // If we have idle time until the next process' arrival
        if (!cpu.isBusy() && currentTime < (*currentProcess).get_arr_time() 
            && eventToExecute.get_eventType() == process_arrival){
            currentTime = (*currentProcess).get_arr_time();
        }

        

        if (eventToExecute.get_eventType() == process_arrival){

            // Add process to readyQueue
            readyQueue.push(*currentProcess);

        } else if (eventToExecute.get_eventType() == process_departure){

            cpu.setIdle();
            completedProcesses++;

        } else if (eventToExecute.get_eventType() == quantum_expiration){

            // If there's remaining service time, send back to readyQueue.
            // Else, create departure event.

            if ((*currentProcess).get_remaining_svc_time() > 0.0){
                readyQueue.push(*currentProcess);
            } else {
                Event departureEvent = Event(process_departure, currentProcess);
                eventQueue.push(departureEvent);
            }

            cpu.setIdle();

        } else { // ERROR: Invalid event type
            cout << "\n******************************";
            cout << "\nERROR: Invalid event type!\n";
            cout << "\n******************************\n";
        }

        // Execute next process, if any
        if (readyQueue.size() > 0){

            // Grab next process
            currentProcess = &readyQueue.front();
            readyQueue.pop();

            // Simulate execution, advance clock, create quant_expiration event
            double quantum = calculator.calculate_quantum(base_quant, A, B, currentProcess);
            currentProcess->do_execution(quantum);
            currentTime += quantum;
            Event quantExpireEvent = Event(quantum_expiration, currentProcess);
        }
    }
    


}

