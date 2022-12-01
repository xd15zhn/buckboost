#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "simucpp.hpp"
using namespace simucpp;

class PID_Controller: public PackModule
{
public:
    PID_Controller() {};
    PID_Controller(Simulator *sim, double Kp=1, double Ki=0):_Kp(Kp), _Ki(Ki) {
        Initialize(sim);
    };
    virtual PUnitModule Get_InputPort(int n) const {
        if (n==0) return gainin;
        return nullptr;
    };
    virtual PUnitModule Get_OutputPort(int n) const {
        if (n==0) return sumout1;
        return nullptr;
    };
    void Initialize(Simulator *sim){
        int1 = new UIntegrator(sim, "int1");
        gainin = new UGain(sim, "gainin");
        sumout1 = new USum(sim, "sumout1");
        sim->connectU(gainin, int1);
        sim->connectU(gainin, sumout1);
        sim->connectU(int1, sumout1);
        sumout1->Set_InputGain(_Kp, 0);
        sumout1->Set_InputGain(_Ki, 1);
    }
    double _Kp=1, _Ki=0;
private:
    UIntegrator *int1;
    UGain *gainin=nullptr;
    USum *sumout1;
};

#endif // PID_CONTROLLER_H
