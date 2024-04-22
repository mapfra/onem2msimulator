// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_HOSTROUTER_H_
#define __ONEM2MSIMULATOR_HOSTROUTER_H_

#include <omnetpp.h>

#include "../../../../messages/OneM2MPrimitive_m.h"
#include "../../../../messages/DBMessage_m.h"
#include "../../../../messages/NetworkPacket_m.h"

#include "../../../../lib/constants.hpp"
#include "../../../../lib/messaging.hpp"

using namespace omnetpp;
using namespace std;


class HostRouter : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    map<string, int> *upperGateIndex;
    map<string, int> *lowerGateIndex;

};

#endif
