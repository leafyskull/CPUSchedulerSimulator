#ifndef CPU_H
#define CPU_H


class CPU{
    private:
        bool busy;
    public:
        CPU();
        bool isBusy();
        void setBusy();
        void setIdle();

};

#endif