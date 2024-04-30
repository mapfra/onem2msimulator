// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "NIC.h"

Define_Module(NIC);

void NIC::initialize()
{
    this->portQueue = new cPacketQueue();
    this->portScheduler = new cMessage("timer", PacketKind::TIMER);
}

void NIC::handleMessage(cMessage *msg)
{
    if (msg->isName("timer")) {
        NetworkPacket* pk = (NetworkPacket*)this->portQueue->pop();
        send(pk, "downLink$o");

        if (this->portQueue->getLength()>0) {
            simtime_t targetDate = gate("downLink$o")->getTransmissionChannel()->getTransmissionFinishTime();
            scheduleAt(targetDate, msg);
        }
    }
    else
    {
        if (msg->arrivedOn("upLink$i")){
            NetworkPacket* netPacket = dynamic_cast<NetworkPacket*>(msg);
            simtime_t targetDate = gate("downLink$o")->getTransmissionChannel()->getTransmissionFinishTime();
            if (targetDate <= simTime()){
                send(netPacket, "downLink$o");
            }
            else {
                portQueue->insert(netPacket);
                if (!portScheduler->isScheduled())
                    scheduleAt(targetDate, portScheduler);
            }
        }
        else {
            if (msg->arrivedOn("downLink$i"))
                send(msg, "upLink$o");
        }
    }
}

NIC::~NIC(){
    cancelAndDelete(this->portScheduler);
    while(this->portQueue->getLength()>0) {
        cPacket* pkt = this->portQueue->pop();
        cancelAndDelete(pkt);
    }
    delete this->portQueue;

}
