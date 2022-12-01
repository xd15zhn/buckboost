#include <iostream>
#include "simucpp.hpp"
using namespace simucpp;
using namespace std;

constexpr double Vin0 = 60;  // 输入电压(V)
constexpr double Vref = 40;  // 输出电压设定值(V)
constexpr double f0 = 85000;  // 开关频率(Hz)
constexpr double T0 = 1e6/f0;  // 开关周期(us)
constexpr double L0 = 1e-4;  // 开关频率(Hz)
constexpr double C0 = 1e-6;  // 电容(F)
constexpr double R0 = 100;  // 负载电阻(ou)
constexpr double RATE = 1e-6;  // 时间单位为微秒

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
        misovo->Set_Function([](double *u){
            double vin = u[0];
            double vo  = u[1];
            double iL  = u[2];
            return (-vo/(R0*C0) + vin*iL/C0)*RATE;
        });
        misoiL->Set_Function([](double *u){
            double vin = u[0];
            double vo  = u[1];
            return (-vin/L0*vo + (1-vin)/L0*Vin0)*RATE;
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
};

int main()
{
    // 初始化仿真器
    Simulator sim1(1000);
    auto *plant = new BuckBoost(&sim1, "plant");
    auto *cnstDuty = new UConstant(&sim1, "cnstDuty");
    auto *outVin = new UOutput(&sim1, "outVin");
    auto *outvo = new UOutput(&sim1, "outvo");
    auto *outiL = new UOutput(&sim1, "outiL");

    // 初始化模块
    sim1.connectU(cnstDuty, plant, 0);
    sim1.connectU(plant, 0, outVin);
    sim1.connectU(plant, 1, outvo);
    sim1.connectU(plant, 2, outiL);
    cnstDuty->Set_OutValue(0.4);
    sim1.Set_SampleTime(1);
    outVin->Set_EnableStore(false);
    // outvo->Set_EnableStore(false);
    outiL->Set_EnableStore(false);
    // outVin->Set_InputGain(10);
    // outiL->Set_InputGain(10);

    // 运行仿真
    sim1.Initialize();
    sim1.Simulate();
    sim1.Plot();
    return 0;
}
