#include "src/buckboost.hpp"
#include <iostream>
using namespace std;

int main()
{
    Simulator sim1(1000);
    auto *plant = new BuckBoost(&sim1, "plant");
    auto *cnstDuty = new UConstant(&sim1, "cnstDuty");
    auto *outVin = new UOutput(&sim1, "outVin");
    auto *outvo = new UOutput(&sim1, "outvo");
    auto *outiL = new UOutput(&sim1, "outiL");

    sim1.connectU(cnstDuty, plant, 0);
    sim1.connectU(plant, 0, outVin);
    sim1.connectU(plant, 1, outvo);
    sim1.connectU(plant, 2, outiL);
    cnstDuty->Set_OutValue(0.4);
    sim1.Set_SampleTime(1);
    outVin->Set_EnableStore(false);
    outiL->Set_EnableStore(false);

    sim1.Initialize();
    sim1.Simulate();
    sim1.Plot();
    return 0;
}
