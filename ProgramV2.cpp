#include <iostream>
#include <random>
#include <queue>
#include "Event.h"
#include "Process.h"
#include "Calculations.h"
#include "CPU.h"
#include "Calculations.h"
using namespace std;

void Simulator(double avg_arr_rate, double avg_svc_time, double base_quant, double A, double B);

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

void Simulator(double avg_arr_rate, double avg_svc_time, double base_quant, double A, double B){
    
    // Need to track:
    // - Average turnaround time
    //   Have each process track turnaround time, and average at end?
    // - Average number of context switches
    //   Have each process track their number of arrivals?
    // - Average number of processes in the ready queue
    //   ?

    cout << endl;
    cout << "> Simulator starting..." << endl;

    Calculations calculator;

    queue<Process> readyQueue; // Ready queue of processes
    priority_queue<Event> eventQueue; // Queue of events
    CPU cpu; // CPU we will use

    double currentTime = 0.0; // Total time clock
    double busyTime = 0.0; // Total time CPU spent executing
    double generatedTime = 0.0; // For generating the process arrival times

    

    // 1. Generate a workload of 10,000 processes
    const int NUM_PROCESSES = 3;
    int completedProcesses = 0;
    Process processes[NUM_PROCESSES];
    double turnaroundTimes[NUM_PROCESSES];

    double service_time, local_arrival_time;
    int priority;

    cout << "> Generating processes..." << endl << endl;

    for (int i = 0; i < NUM_PROCESSES; i++){
        service_time = calculator.calculate_service_time(avg_svc_time);
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
        cout << "Svc time: " << processes[i].get_remaining_svc_time() << " seconds" << endl;
        cout << "Arr time: " << processes[i].get_arr_time() << " seconds" << endl;
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

        cout << "****************************************" << endl;
        cout << "Handling event: " << eventToExecute.get_eventType_as_string() <<
                " for PID: " << to_string((*currentProcess).get_PID()) << endl; 
        cout << endl;



        if (eventToExecute.get_eventType() == process_arrival){

            // If CPU is free, schedule service event now.
            if (!cpu.isBusy()){
                cout << "CPU is free, scheduling service event now..." << endl;
                Event serviceEvent = Event(service_arrival, currentProcess, currentTime);
                eventQueue.push(serviceEvent);
                readyQueue.push(*currentProcess);
            } else { // Else, add to readyQueue.
                cout << "CPU is busy, adding process to ready queue..." << endl;
                readyQueue.push(*currentProcess);
            }

        } else if (eventToExecute.get_eventType() == process_departure){

            cpu.setIdle();
            completedProcesses++;
            cout << "PID " << to_string((*currentProcess).get_PID()) << " has finished. Departing..." << endl;

            // Get current process' turnaround time
            double currProcTurnaroundTime = currentTime - (*currentProcess).get_arr_time();
            turnaroundTimes[(*currentProcess).get_PID()] = currProcTurnaroundTime;

        } else if (eventToExecute.get_eventType() == service_arrival){

            // TEST
            if ((*currentProcess).get_PID() != (readyQueue.front()).get_PID()){
                cout << "Error! Current process is not at front of readyQueue!" << endl;
            } else {
                cout << "CurrentProcess is at the front of readyQueue. Good!" << endl;
                readyQueue.pop();
            }

            cout << "PID " << to_string((*currentProcess).get_PID()) << " has arrived for service. Servicing..." << endl;

            cpu.setBusy();
            
            // Calculate quantum and do execution
            double quantum = calculator.calculate_quantum(base_quant, A, B, currentProcess);
            cout << "Assigning quantum of " << to_string(quantum) << " seconds" << endl;
            double init_remaining_svc_time = (*currentProcess).get_remaining_svc_time();
            (*currentProcess).do_execution(quantum);
            
            if (init_remaining_svc_time < quantum)
                busyTime += init_remaining_svc_time;
            else
                busyTime += quantum;

            // Schedule quantum_expiration event
            cout << "Scheduling quantum expiration event..." << endl;
            Event quantumExpEvent = Event(quantum_expiration, currentProcess, currentTime + quantum);
            eventQueue.push(quantumExpEvent);

        } else if (eventToExecute.get_eventType() == quantum_expiration){

            cout << "PID " << to_string((*currentProcess).get_PID()) << "'s quantum has finished." << endl;
            cout << "Remaining service time: " << to_string((*currentProcess).get_remaining_svc_time()) << " seconds" << endl;

            // Figure out if process needs more work or is done.
            if ((*currentProcess).get_remaining_svc_time() > 0.0){
                cout << "Putting process back in ready queue..." << endl;
                readyQueue.push(*currentProcess);
            } else {
                cout << "Creating departure event..." << endl;
                Event departureEvent = Event(process_departure, currentProcess, currentTime);
                eventQueue.push(departureEvent);
            }

            cpu.setIdle();

            // Schedule service arrival of next process in readyQueue if there is one
            if (!readyQueue.empty()){
                Process* nextProcess = &(readyQueue.front());
                cout << "Scheduling service arrival of next process (PID " << to_string((*nextProcess).get_PID()) << ") in ready queue..." << endl;
                Event nextProcessServiceArrival = Event(service_arrival, nextProcess, currentTime);
                eventQueue.push(nextProcessServiceArrival);
            }

        } else { // ERROR: Invalid event type
            cout << "\n******************************" << endl;;
            cout << "ERROR: Invalid event type!" << endl;;
            cout << "Event type: " << eventToExecute.get_eventType_as_string() << endl;
            cout << "******************************\n" << endl;;
        }

        cout << "****************************************" << endl << endl;

    }

    // Calculate average turnaround time
    double sum = 0.0;
    for (int i = 0; i < NUM_PROCESSES; i++){
        sum += turnaroundTimes[i];
    }
    double avgTurnaroundTime = sum / NUM_PROCESSES;

    cout << "> Simulator has finished running." << endl;
    cout << "Average turnaround time: " << to_string(avgTurnaroundTime) << " seconds" << endl;
    cout << endl;
}

