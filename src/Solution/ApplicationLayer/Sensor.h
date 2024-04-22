// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef __ONEM2MSIMULATOR_SENSOR_H_
#define __ONEM2MSIMULATOR_SENSOR_H_

#include <omnetpp.h>
#include <fstream>

#include "../../messages/OneM2MPrimitive_m.h"

#include "../../lib/constants.hpp"
#include "../../lib/json.hpp"
#include "../../lib/serialization.hpp"
#include "../../lib/messaging.hpp"

using namespace omnetpp;
using namespace std;
using json = nlohmann::json;


class Sensor : public cSimpleModule
{
  private:
     string name;
     int protocol;
     EventGenerator* cinGenerator;
     int cinID = 0;

     map<int, double> sendingTime;
     cOutVector cinRTT;


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void runInitialProcedure();
};

#endif
