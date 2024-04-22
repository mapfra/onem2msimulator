// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_INTERNET_H_
#define __ONEM2MSIMULATOR_INTERNET_H_

#include <omnetpp.h>

using namespace omnetpp;
using namespace std;


class Internet : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    map<string, int> * gateIndex;
};

#endif
