// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef __ONEM2MSIMULATOR_HTTPBINDING_H_
#define __ONEM2MSIMULATOR_HTTPBINDING_H_

#include <omnetpp.h>

#include "../../../../lib/constants.hpp"

using namespace omnetpp;

class HTTPBinding : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
