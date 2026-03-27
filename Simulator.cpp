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

    bool runWithDefaultParameters = true;

    if (runWithDefaultParameters){
        average_arrival_rate = 12.0;
        average_service_rate = 0.06;
        base_quantum = 0.03;
        scaling_factor_A = 100;
        scaling_factor_B = 1;
    }

    else{
        cout << "Please input parameters in the following format: " << endl;
        cout << "[Avg arrival rate] [avg service time] [base quantum] [scaling factor A] [scaling factor B]" << endl;
        cout << "Ex: 12 0.06 0.03 100 1" << endl;
        cout << "Input: ";
        cin >> average_arrival_rate >> average_service_rate >>
               base_quantum >> scaling_factor_A >> scaling_factor_B;
    }
    
    Simulator(average_arrival_rate, average_service_rate, base_quantum, 
              scaling_factor_A, scaling_factor_B);

    return 0;
}

void Simulator(double avg_arr_rate, double avg_svc_time, double base_quant, double A, double B){
    
    bool doDebugText = false;

    cout << endl;
    cout << "> Simulator starting..." << endl;

    Calculations calculator;

    // TODO: Replace with CPU's readyQueue
    // queue<Process*> readyQueue; // Ready queue of processes
    
    priority_queue<Event> eventQueue; // Queue of events
    CPU cpu; // CPU we will use

    double currentTime = 0.0; // Total time clock
    double busyTime = 0.0; // Total time CPU spent executing
    double generatedTime = 0.0; // For generating the process arrival times

    // For calculating average readyQueue size:
    // At each service, multiply (currentTime - previousTime) * readyQueue.size()
    // and add to serviceTimeByReadyQueueSizeSum.
    // At the end of the simulation, we'll divide this by the total elapsed time.
    double previousServiceTime = 0.0;
    double serviceTimeByReadyQueueSizeSum = 0.0;

    

    // Generate a workload of 10,000 processes
    const int NUM_PROCESSES = 10000;
    int completedProcesses = 0;
    int contextSwitches = 0;

    vector<Process> processes(NUM_PROCESSES);
    vector<double> turnaroundTimes(NUM_PROCESSES);

    double service_time, local_arrival_time;
    int priority;

    cout << "> Generating processes..." << endl;

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

        if (doDebugText){
            cout << "******************************" << endl;
            cout << "Process generated!" << endl;
            cout << "PID: " << processes[i].get_PID() << endl;
            cout << "Svc time: " << processes[i].get_remaining_svc_time() << " seconds" << endl;
            cout << "Arr time: " << processes[i].get_arr_time() << " seconds" << endl;
            cout << "Static priority: " << processes[i].get_priority() << endl;
            cout << "******************************" << endl << endl;
        }
    }

    Process* currentProcess = nullptr;
    Process* previousProcess = nullptr;

    cout << "> Running simulation..." << endl << endl;

    // MAIN LOOP
    while (completedProcesses < NUM_PROCESSES){

        // Grab next event in eventQueue
        Event eventToExecute = eventQueue.top();
        eventQueue.pop();

        currentProcess = eventToExecute.get_process();
        currentTime = eventToExecute.getTime();

        if (doDebugText){
            cout << "****************************************" << endl;
            cout << "Handling event: " << eventToExecute.get_eventType_as_string() <<
                    " for PID: " << to_string((*currentProcess).get_PID()) << endl; 
            cout << endl;
        }
        



        if (eventToExecute.get_eventType() == process_arrival){

            // If CPU is free, schedule service event now.
            if (!cpu.isBusy()){
                if (doDebugText) cout << "CPU is free, scheduling service event now..." << endl;
                Event serviceEvent = Event(service_arrival, currentProcess, currentTime);
                eventQueue.push(serviceEvent);
                cpu.readyQueuePush(currentProcess);
            } else { // Else, add to readyQueue.
                if (doDebugText) cout << "CPU is busy, adding process to ready queue..." << endl;
                cpu.readyQueuePush(currentProcess);
            }

        } else if (eventToExecute.get_eventType() == process_departure){

            cpu.setIdle();
            completedProcesses++;
            if (doDebugText) cout << "PID " << to_string((*currentProcess).get_PID()) << " has finished. Departing..." << endl;

            // Get current process' turnaround time
            double currProcTurnaroundTime = currentTime - (*currentProcess).get_arr_time();
            turnaroundTimes[(*currentProcess).get_PID()] = currProcTurnaroundTime;

        } else if (eventToExecute.get_eventType() == service_arrival){

            serviceTimeByReadyQueueSizeSum += (currentTime - previousServiceTime) * cpu.readyQueueSize();
            previousServiceTime = currentTime;

            // Determine if a context switch occurred
            if (previousProcess == nullptr) 
                if (doDebugText) cout << "Loading first process for service. Not counting this as a context switch." << endl;
            if (previousProcess != nullptr && currentProcess != previousProcess){
                contextSwitches++;
                if (doDebugText) cout << "Context switch happened! From PID " << to_string((*previousProcess).get_PID()) << " to PID " << to_string((*currentProcess).get_PID()) << endl;
            }

            // Take process out of readyQueue
            if (currentProcess != cpu.readyQueueFront()){
                cout << "Error! Current process is not at front of readyQueue!" << endl;
            } else {
                if (doDebugText) cout << "CurrentProcess is at the front of readyQueue. Good!" << endl;
                cpu.readyQueuePop();
            }

            if (doDebugText) cout << "PID " << to_string((*currentProcess).get_PID()) << " has arrived for service. Servicing..." << endl;

            cpu.setBusy();
            
            // Calculate quantum and do execution
            double quantum = calculator.calculate_quantum(base_quant, A, B, currentProcess);
            if (doDebugText) cout << "Assigning quantum of " << to_string(quantum) << " seconds" << endl;
            double init_remaining_svc_time = (*currentProcess).get_remaining_svc_time();
            (*currentProcess).do_execution(quantum);
            
            if (init_remaining_svc_time < quantum)
                busyTime += init_remaining_svc_time;
            else
                busyTime += quantum;

            // Schedule quantum_expiration event
            if (doDebugText) cout << "Scheduling quantum expiration event..." << endl;
            Event quantumExpEvent = Event(quantum_expiration, currentProcess, currentTime + quantum);
            eventQueue.push(quantumExpEvent);

            previousProcess = currentProcess;

        } else if (eventToExecute.get_eventType() == quantum_expiration){

            if (doDebugText) {
                cout << "PID " << to_string((*currentProcess).get_PID()) << "'s quantum has finished." << endl;
                cout << "Remaining service time: " << to_string((*currentProcess).get_remaining_svc_time()) << " seconds" << endl;
            }

            // Figure out if process needs more work or is done.
            if ((*currentProcess).get_remaining_svc_time() > 0.0){
                if (doDebugText) cout << "Putting process back in ready queue..." << endl;
                cpu.readyQueuePush(currentProcess);
            } else {
                if (doDebugText) cout << "Creating departure event..." << endl;
                Event departureEvent = Event(process_departure, currentProcess, currentTime);
                eventQueue.push(departureEvent);
            }

            cpu.setIdle();

            // Schedule service arrival of next process in readyQueue if there is one
            if (!cpu.readyQueueIsEmpty()){
                Process* nextProcess = cpu.readyQueueFront();
                if (doDebugText) cout << "Scheduling service arrival of next process (PID " << to_string((*nextProcess).get_PID()) << ") in ready queue..." << endl;
                Event nextProcessServiceArrival = Event(service_arrival, nextProcess, currentTime);
                eventQueue.push(nextProcessServiceArrival);
            }

        } else { // ERROR: Invalid event type
            cout << "\n******************************" << endl;;
            cout << "ERROR: Invalid event type!" << endl;;
            cout << "Event type: " << eventToExecute.get_eventType_as_string() << endl;
            cout << "******************************\n" << endl;;
        }

        if (doDebugText) {
            cout << endl;
            cout << "Event handling finished!" << endl << endl;
        }

    }

    // Calculate average turnaround time
    double sum = 0.0;
    for (int i = 0; i < NUM_PROCESSES; i++){
        sum += turnaroundTimes[i];
    }
    double avgTurnaroundTime = sum / NUM_PROCESSES;

    // Calculate average number of processes in the readyQueue
    double avgNumProcessesInReadyQueue = serviceTimeByReadyQueueSizeSum / currentTime;

    cout << "************************************************************" << endl;
    cout << "> Simulator has finished running." << endl;
    cout << "> RESULTS: " << endl;
    cout << "> Completed processes: " << to_string(completedProcesses) << "/" << to_string(NUM_PROCESSES) << endl;
    cout << "> Average turnaround time: " << to_string(avgTurnaroundTime) << " seconds" << endl;
    cout << "> Average number of context switches per process: " << to_string((static_cast<double>(contextSwitches) / static_cast<double>(NUM_PROCESSES))) << endl;
    cout << "> Average number of processes in the readyQueue: " << to_string(avgNumProcessesInReadyQueue) << endl;
    cout << "************************************************************" << endl;
    cout << endl;
}

