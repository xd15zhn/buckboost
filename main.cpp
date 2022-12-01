#include "src/buckboost.hpp"
#include "src/pidcontroller.hpp"
#include <iostream>
using namespace std;

int main()
{
    Simulator sim1(1000);
    auto *plant = new BuckBoost(&sim1, "plant");
    auto *pid = new PID_Controller(&sim1, 1, 0);
    auto *sum1 = new USum(&sim1, "sum1");
    auto *cnstin1 = new UConstant(&sim1, "cnstin1");
    FUOutput(outVo, &sim1);
    FUOutput(outerr, &sim1);

    sim1.connectU(cnstin1, sum1);
    sim1.connectU(sum1, pid, 0);
    sim1.connectU(pid, 0, plant, 0);
    sim1.connectU(plant, 0, sum1);
    sum1->Set_InputGain(-1);
    sim1.connectU(plant, 1, outVo);
    sim1.connectU(sum1, outerr);
    cnstin1->Set_OutValue(Vref);
    sim1.Set_SampleTime(1);

    sim1.Initialize();
    sim1.Simulate();
    sim1.Plot();
    return 0;
}
