// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_TCPBINDING_H_
#define __ONEM2MSIMULATOR_TCPBINDING_H_

#include <omnetpp.h>

using namespace omnetpp;


class TCPBinding : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
