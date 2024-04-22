// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_CPUMANAGER_H_
#define __ONEM2MSIMULATOR_CPUMANAGER_H_

#include <omnetpp.h>

using namespace omnetpp;


class CPUManager : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) {};

  public:
    int IPS;
    double nextAvailablity;

    double addTask(int instrCount);
    double getProcessingTime(int instrCount);

};

#endif
