// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_DATASTORAGE_H_
#define __ONEM2MSIMULATOR_DATASTORAGE_H_

#include <omnetpp.h>

#include "../../../InfrastructureLayer/Nodes/Components/CPUManager.h"
#include "../../../InfrastructureLayer/Nodes/Components/IOManager.h"
#include "../../../InfrastructureLayer/Nodes/Components/RAMManager.h"

#include "../../../../messages/OneM2MPrimitive_m.h"
#include "../../../../messages/DBMessage_m.h"

#include "../../../../lib/onem2m_resources.hpp"
#include "../../../../lib/serialization.hpp"
#include "../../../../lib/messaging.hpp"
#include "../../../../lib/constants.hpp"

#include <queue>





using namespace omnetpp;
using namespace std;


class DataStorage : public cSimpleModule
{
  public:
    ~DataStorage();

  protected:

    CPUManager* cpu;
    RAMManager* ram;
    IOManager* io;
    PerformanceDescriptor *perf;

    bool isStandAlone;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;


  private:
    cPacketQueue* waiting;
    cPacketQueue* processing;

    void handleStoragePacket(DBMessage* dbPkt);
    void handleTimer(cMessage* timer);

    void scheduleCreateResource(DBMessage* dbPkt);
    void scheduleRetrieveResource(DBMessage* dbPkt);
    void scheduleUpdateResource(DBMessage* dbPkt);
    void scheduleDeleteResource(DBMessage* dbPkt);

};

#endif
