// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef __ONEM2MSIMULATOR_CORE_H_
#define __ONEM2MSIMULATOR_CORE_H_

#include <omnetpp.h>

#include "../../InfrastructureLayer/Nodes/Components/CPUManager.h"
#include "../../InfrastructureLayer/Nodes/Components/IOManager.h"
#include "../../InfrastructureLayer/Nodes/Components/RAMManager.h"

#include "../../../messages/OneM2MPrimitive_m.h"
#include "../../../messages/DBMessage_m.h"

#include "../../../lib/constants.hpp"
#include "../../../lib/onem2m_resources.hpp"
#include "../../../lib/serialization.hpp"
#include "../../../lib/messaging.hpp"

#include <queue>
#include <string.h>




using namespace omnetpp;
using namespace std;


class Core : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    CPUManager* cpu;
    RAMManager* ram;
    IOManager* io;
    PerformanceDescriptor *perf;
    Resource* resourceTree;


    string cseName;
    bool hasHTTP;
    bool hasCOAP;
    bool hasMQTT;
    bool hasLocalDB;
    string dbServerAddress;
    string remoteCSE;
    string remoteCSEAddress;



    map<int, double> receivedDate;
    cOutVector processingTime;

  private:
    cPacketQueue* waiting;
    cPacketQueue* processing;

    void handleTimer(cMessage* timer);
    void handleOneM2MPacket(OneM2MPrimitive* dbPkt);
    void handleStoragePacket(DBMessage* dbPkt);

    void scheduleCreateResource(OneM2MPrimitive* pkt);
    void scheduleRetrieveResource(OneM2MPrimitive* pkt);
    void scheduleUpdateResource(OneM2MPrimitive* pkt);
    void scheduleDeleteResource(OneM2MPrimitive* pkt);

    void createResource(OneM2MPrimitive* pkt);
    void retrieveResource(OneM2MPrimitive* pkt);
    void updateResource(OneM2MPrimitive* pkt);
    void deleteResource(OneM2MPrimitive* pkt);

};

#endif
