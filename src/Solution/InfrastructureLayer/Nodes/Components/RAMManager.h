// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_RAMMANAGER_H_
#define __ONEM2MSIMULATOR_RAMMANAGER_H_

#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

class RAMManager : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) {}


    cOutVector memoryUsage;

  public:
    int maximumBytes;
    int currentBytes;
    bool isAtFullCapacity;
    void addTask(int bytes);
    void removeTask(int bytes);
};

#endif
