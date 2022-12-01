#ifndef PARAMETER_H
#define PARAMETER_H

constexpr double Vin0 = 60;  // 输入电压(V)
constexpr double Vref = 40;  // 输出电压设定值(V)
constexpr double f0 = 85000;  // 开关频率(Hz)
constexpr double T0 = 1e6/f0;  // 开关周期(us)
constexpr double R0 = 100;  // 估计负载电阻(ou)
constexpr double L0 = 1e-4;  // 估计电感(H)
constexpr double C0 = 1e-6;  // 估计电容(F)
constexpr double RATE = 1e-6;  // 时间单位为微秒

#endif // PARAMETER_H
