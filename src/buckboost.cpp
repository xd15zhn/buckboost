#include "buckboost.hpp"
#include "parameter.hpp"

BuckBoost::BuckBoost(Simulator *sim, std::string name) {
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
PUnitModule BuckBoost::Get_InputPort(int n) const {
    if (n==0) return gainDuty;
    return nullptr;
}
PUnitModule BuckBoost::Get_OutputPort(int n) const {
    if (n==0) return fcnSwitchVin;
    if (n==1) return intvo;
    if (n==2) return intiL;
    return nullptr;
}
