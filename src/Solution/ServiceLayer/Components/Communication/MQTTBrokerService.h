// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_MQTTBROKERSERVICE_H_
#define __ONEM2MSIMULATOR_MQTTBROKERSERVICE_H_

#include <omnetpp.h>

using namespace omnetpp;


class MQTTBrokerService : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
