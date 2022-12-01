#ifndef BUCKBOOST_H
#define BUCKBOOST_H

#include "simucpp.hpp"
using namespace simucpp;

class BuckBoost: public PackModule
{
public:
    BuckBoost(Simulator *sim, std::string name="BuckBoost");
    virtual PUnitModule Get_InputPort(int n=0) const;
    virtual PUnitModule Get_OutputPort(int n=0) const;
private:
    UGain *gainDuty=nullptr;
    UInput *inTriangle=nullptr;
    UFcnMISO *fcnSwitchVin=nullptr;
    UIntegrator *intvo=nullptr;
    UIntegrator *intiL=nullptr;
    UFcnMISO *misovo=nullptr;
    UFcnMISO *misoiL=nullptr;
    double _R=0.9*R0;  // 实际电阻
    double _L=0.9*L0;  // 实际电感
    double _C=0.9*C0;  // 实际电容
};

#endif // BUCKBOOST_H
