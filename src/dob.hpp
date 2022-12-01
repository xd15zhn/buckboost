#ifndef DOB_H
#define DOB_H

#include "simucpp.hpp"
using namespace simucpp;

class DOB1: public PackModule
{
public:
    DOB1(Simulator *sim, std::string name="DOB1");
    virtual PUnitModule Get_InputPort(int n=0) const;
    virtual PUnitModule Get_OutputPort(int n=0) const;
private:
    UIntegrator *intz=nullptr;
    UFcnMISO *misoz=nullptr;
    UFcnMISO *misod=nullptr;
};

class DOB2: public PackModule
{
public:
    DOB2(Simulator *sim, std::string name="DOB2");
    virtual PUnitModule Get_InputPort(int n=0) const;
    virtual PUnitModule Get_OutputPort(int n=0) const;
private:
    UIntegrator *intz=nullptr;
    UFcnMISO *misoz=nullptr;
    UFcnMISO *misod=nullptr;
};

#endif // DOB_H
