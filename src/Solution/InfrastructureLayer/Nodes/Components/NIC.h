// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_NIC_H_
#define __ONEM2MSIMULATOR_NIC_H_

#include <omnetpp.h>

#include "../../../../lib/constants.hpp"
#include "../../../../messages/NetworkPacket_m.h"

using namespace omnetpp;


class NIC : public cSimpleModule
{
  public:
    ~NIC();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    cPacketQueue* portQueue;
    cMessage* portScheduler;
};

#endif
