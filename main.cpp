#include <iostream>
#include "simucpp.hpp"
using namespace simucpp;
using namespace std;

constexpr double Vin0 = 60;  // 输入电压(V)
constexpr double Vref = 40;  // 输出电压设定值(V)
constexpr double f0 = 85000;  // 开关频率(Hz)
constexpr double T0 = 1e6/f0;  // 开关周期(us)
constexpr double duty = 0.4;  // 占空比
constexpr double L0 = 1e-4;  // 开关频率(Hz)
constexpr double C0 = 1e-6;  // 电容(F)
constexpr double R0 = 100;  // 负载电阻(ou)
constexpr double RATE = 1e-6;  // 时间单位为微秒

class DutyCycle_To_Switch: public PackModule
{
public:
    DutyCycle_To_Switch(Simulator *sim, std::string name="DutyCycle_To_Switch") {
        gainDuty = new UGain(sim, "gainDuty");
        inTriangle = new UInput(sim, "inTriangle");
        fcnSwitchVin = new UFcnMISO(sim, "fcnSwitchVin");
        sim->connectU(gainDuty, fcnSwitchVin);
        sim->connectU(inTriangle, fcnSwitchVin);
        inTriangle->Set_Function([](double u){ return u/11.0 - int(u/11.0); });
        fcnSwitchVin->Set_Function([](double *u){ return u[0]<u[1] ? 0.0 : 1.0; });
    }
    virtual PUnitModule Get_InputPort(int n=0) const {
        if (n==0) return gainDuty;
        return nullptr;
    }
    virtual PUnitModule Get_OutputPort(int n=0) const {
        if (n==0) return fcnSwitchVin;
        return nullptr;
    }
private:
    UGain *gainDuty=nullptr;
    UInput *inTriangle=nullptr;
    UFcnMISO *fcnSwitchVin=nullptr;
};

int main()
{
    // 初始化仿真器
    Simulator sim1(50);

    // 初始化模块
    auto *intvo = new UIntegrator(&sim1, "intvo");
    auto *intiL = new UIntegrator(&sim1, "intiL");
    auto *misovo = new UFcnMISO(&sim1);
    auto *misoiL = new UFcnMISO(&sim1);
    auto *cnstDuty = new UConstant(&sim1, "cnstDuty");
    auto *Wave = new DutyCycle_To_Switch(&sim1, "Wave");
    auto *outVin = new UOutput(&sim1, "outVin");
    auto *outvo = new UOutput(&sim1, "outvo");
    auto *outiL = new UOutput(&sim1, "outiL");
    sim1.connectU(cnstDuty, Wave, 0);
    sim1.connectU(Wave, 0, misovo);
    sim1.connectU(intvo, misovo);
    sim1.connectU(intiL, misovo);
    sim1.connectU(misovo, intvo);
    sim1.connectU(Wave, 0, misoiL);
    sim1.connectU(intvo, misoiL);
    sim1.connectU(intiL, misoiL);
    sim1.connectU(misoiL, intiL);
    sim1.connectU(Wave, 0, outVin);
    sim1.connectU(intvo, outvo);
    sim1.connectU(intiL, outiL);
    // sim1.Set_SampleTime(1);
    // outVin->Set_EnableStore(false);
    outvo->Set_EnableStore(false);
    outiL->Set_EnableStore(false);
    // outVin->Set_InputGain(10);
    // outiL->Set_InputGain(10);
    cnstDuty->Set_OutValue(0.1);
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

    // 运行仿真
    sim1.Initialize();
    sim1.Simulate();
    sim1.Plot();
    return 0;
}
