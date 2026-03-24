
class Process{
    private:
        int PID;
        double arrival_time;
        double total_service_time;
        double remaining_service_time;
        int static_priority;
        double total_execution_time;
        
    public:
        Process();
        Process(int PID, double arr_time, double svc_time, int priority);

        void set_PID(int PID);
        void set_arr_time(double arr_time);
        void set_svc_time(double svc_time);
        void set_priority(int priority);

        int get_PID();
        double get_arr_time();
        double get_remaining_svc_time();
        int get_priority();
        double get_total_execution_time();

        void do_execution(double amount);
};