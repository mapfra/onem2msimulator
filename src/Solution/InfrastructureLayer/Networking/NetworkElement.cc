// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "NetworkElement.h"

Define_Module(NetworkElement);


const int LINK_OUTPUT = 1;
const int INTERNET_OUTPUT =2;

void NetworkElement::initialize()
{
    this->gateIndex = new map<string, int>();
    this->portQueue = new map<int, cPacketQueue*>();
    this->portScheduler = new map<int, cMessage*>();

    int linkCount = gateSize("link$i");
    for (int i=0; i<linkCount; i++) {
        cGate* prevGate = gate("link$i", i)->getPreviousGate();
        cModule* nic = prevGate->getPreviousGate()->getOwnerModule();
        string ip = nic->par("networkAddress").stdstringValue();
        this->gateIndex->insert(pair<string, int>(ip, i));
        this->portQueue->insert(pair<int, cPacketQueue*>(i, new cPacketQueue()));
        cMessage* aTimer = new cMessage("timer", PacketKind::TIMER);
        aTimer->addPar("gateIndex").setLongValue(i);
        this->portScheduler->insert(pair<int, cMessage*>(i, aTimer));

        EV << ip << " at gate idx = " << i << '\n';
        cout << ip << " at gate idx = " << i << '\n';
    }
}

void NetworkElement::handleMessage(cMessage *msg)
{
    if (msg->isName("timer")) {
        long gateIndex = msg->par("gateIndex").longValue();

        NetworkPacket* pk = (NetworkPacket*)portQueue->at(gateIndex)->pop();
        string gateName = pk->par("gateName").longValue() == LINK_OUTPUT ? "link$o" : "internet$o";
        send(pk, gateName.c_str(), gateIndex);

        if (portQueue->at(gateIndex)->getLength()>0) {
            simtime_t targetDate = gate(gateName.c_str(), gateIndex)->getTransmissionChannel()->getTransmissionFinishTime();
            scheduleAt(targetDate, msg);
        }
    }
    else
    {
        NetworkPacket* netPacket = dynamic_cast<NetworkPacket*>(msg);
        string destIP = netPacket->getDestAddress();
        int idx = RoutingTools::getHostGateIndex(destIP, gateIndex);
        if (idx != -1){
            simtime_t targetDate = gate("link$o", idx)->getTransmissionChannel()->getTransmissionFinishTime();
            if (targetDate <= simTime()){
                send(netPacket, "link$o", idx);
            } else {
                netPacket->addPar("gateName").setLongValue(LINK_OUTPUT);
                netPacket->addPar("gateIndex").setLongValue(idx);

                portQueue->at(idx)->insert(netPacket);
                cMessage* timer = portScheduler->at(idx);
                if (!timer->isScheduled())
                    scheduleAt(targetDate, timer);
            }
        }
        else
            if (msg->arrivedOn("link$i")) {
                simtime_t targetDate = gate("internetLink$o")->getTransmissionChannel()->getTransmissionFinishTime();
                if (targetDate <= simTime()){
                    send(netPacket, "internetLink$o");
                } else {
                    netPacket->addPar("gateName").setLongValue(INTERNET_OUTPUT);
                    netPacket->addPar("gateIndex").setLongValue(0);
                    portQueue->at(0)->insert(netPacket);
                    cMessage* timer = portScheduler->at(0);
                    if (!timer->isScheduled())
                        scheduleAt(targetDate, timer);
                }
            }
    }
}
