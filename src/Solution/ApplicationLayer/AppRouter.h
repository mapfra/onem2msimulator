// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_APPROUTER_H_
#define __ONEM2MSIMULATOR_APPROUTER_H_

#include <omnetpp.h>

#include "../../messages/NetworkPacket_m.h"
#include "../../messages/OneM2MPrimitive_m.h"


using namespace omnetpp;
using namespace std;


class AppRouter : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    map<string, int> * gateIndex;
};

#endif
