#ifndef BUCKBOOST_H
#define BUCKBOOST_H

#include "parameters.hpp"
#include "simucpp.hpp"
using namespace simucpp;

class BuckBoost: public PackModule
{
public:
    BuckBoost(Simulator *sim, std::string name="BuckBoost") {
        intvo = new UIntegrator(sim, "intvo");  // 负载/电容电压输出
        intiL = new UIntegrator(sim, "intiL");  // 电感电流输出
        misovo = new UFcnMISO(sim);  // 电压积分器输入
        misoiL = new UFcnMISO(sim);  // 电流积分器输入
        gainDuty = new UGain(sim, "gainDuty");  // 输入占空比
        inTriangle = new UInput(sim, "inTriangle");  // 三角波信号源
        fcnSwitchVin = new UFcnMISO(sim, "fcnSwitchVin");  // 使用三角波信号源根据输入占空比输出PWM信号
        misovo->Set_Function([=](double *u){
            double vin = u[0];
            double vo  = u[1];
            double iL  = u[2];
            return (-vo/(_R*_C) + vin*iL/_C)*RATE;
        });
        misoiL->Set_Function([=](double *u){
            double vin = u[0];
            double vo  = u[1];
            return (-vin/_L*vo + (1-vin)/_L*Vin0)*RATE;
        });
        inTriangle->Set_Function([](double u){ return u/T0 - int(u/T0); });
        fcnSwitchVin->Set_Function([](double *u){ return u[0]<u[1] ? 1.0 : 0.0; });
        sim->connectU(gainDuty, fcnSwitchVin);
        sim->connectU(inTriangle, fcnSwitchVin);
        sim->connectU(fcnSwitchVin, misovo);
        sim->connectU(intvo, misovo);
        sim->connectU(intiL, misovo);
        sim->connectU(misovo, intvo);
        sim->connectU(fcnSwitchVin, misoiL);
        sim->connectU(intvo, misoiL);
        sim->connectU(intiL, misoiL);
        sim->connectU(misoiL, intiL);
    }
    virtual PUnitModule Get_InputPort(int n=0) const {
        if (n==0) return gainDuty;
        return nullptr;
    }
    virtual PUnitModule Get_OutputPort(int n=0) const {
        if (n==0) return fcnSwitchVin;
        if (n==1) return intvo;
        if (n==2) return intiL;
        return nullptr;
    }
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
