// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_CSEROUTER_H_
#define __ONEM2MSIMULATOR_CSEROUTER_H_

#include <omnetpp.h>


#include "../../../messages/OneM2MPrimitive_m.h"
#include "../../../messages/DBMessage_m.h"
#include "../../../lib/messaging.hpp"
#include "../../../lib/constants.hpp"


using namespace omnetpp;

class CSERouter : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    bool hasHTTP;
    bool hasMQTT;
    bool hasCOAP;
    bool hasLocalDB;
};

#endif
