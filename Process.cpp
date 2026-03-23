



class Process{
    private:
        int PID;
        double arrival_time;
        double service_time;
        int static_priority;
        double total_execution_time;
    public:
        int get_PID();
        double get_arr_time();
        double get_svc_time();
        int get_priority();
        double get_total_execution_time();

    
    // Constructor
    Process::Process(int PID, double arr_time, double svc_time, int priority){
        this->PID = PID;
        this->arrival_time = arr_time;
        this->service_time = svc_time;
        this->static_priority = priority;
    }


    // > Getters:
    int Process::Get_PID(){
        return this->PID;
    }

    double Process::get_arr_time(){
        return this->arrival_time;
    }

    double Process::get_svc_time(){
        return this->service_time;
    }

    int Process::get_priority(){
        return this->static_priority;
    }

    double Process::get_total_execution_time(){
        return this->total_execution_time;
    }
};