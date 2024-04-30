// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_NETWORKELEMENT_H_
#define __ONEM2MSIMULATOR_NETWORKELEMENT_H_

#include <omnetpp.h>

#include "../../../lib/constants.hpp"
#include "../../../lib/routing.hpp"
#include "../../../messages/NetworkPacket_m.h"

using namespace omnetpp;
using namespace std;


class NetworkElement : public cSimpleModule
{
  public:
    ~NetworkElement();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    map<string, int> * gateIndex;

    map<int, cPacketQueue*>* portQueue;
    map<int, cMessage*>* portScheduler;




};

#endif
