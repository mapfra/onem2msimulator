// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "DataStorage.h"

Define_Module(DataStorage);

void DataStorage::initialize()
{
    this->isStandAlone = par("isStandAlone").boolValue();

    cModule* hwNode = NULL;
    if (this->isStandAlone) // db ^ HWNode v *Manager
        hwNode = getParentModule();
    else  // db ^ CSE ^ CSENode v *Manager
        hwNode = getParentModule()->getParentModule();

    this->cpu = check_and_cast<CPUManager*>(hwNode->getSubmodule("cpuManager"));
    this->ram = check_and_cast<RAMManager*>(hwNode->getSubmodule("ramManager"));
    this->io = check_and_cast<IOManager*>(hwNode->getSubmodule("ioManager"));

    string perfDescriptorString = par("performanceDescriptor");
    json jsonObj = PerformanceDescriptorDeserializer::parseInput(perfDescriptorString);
    this->perf = PerformanceDescriptorDeserializer::getPerformanceDescriptor(jsonObj);

    this->waiting = new cPacketQueue();
    this->processing = new cPacketQueue();
}


void DataStorage::scheduleCreateResource(DBMessage* dbPkt){

    switch (dbPkt->getResourceType()){
        case ResourceType::AE: {
            this->processing->insert(dbPkt);

            int instrCount = this->perf->resourcePerformances.AE.processorUsage.createOperation;
            double procTime = this->cpu->getProcessingTime(instrCount);
            double endProcessing = this->cpu->nextAvailablity + procTime;
            this->cpu->addTask(instrCount);

            int byteCount = this->perf->resourcePerformances.AE.memoryUsage.createOperation;
            this->ram->addTask(byteCount);

            cMessage* timer = new cMessage("timer -> DB -> END CREATE AE", PacketKind::TIMER);
            scheduleAt(simTime()+endProcessing, timer);

        } break;

        case ResourceType::CNT : {

        } break;

        case ResourceType::CIN : {
            this->processing->insert(dbPkt);

            int instrCount = this->perf->resourcePerformances.CIN.processorUsage.createOperation;
            double procTime = this->cpu->getProcessingTime(instrCount);
            double endProcessing = this->cpu->nextAvailablity + procTime;
            this->cpu->addTask(instrCount);

            int byteCount = this->perf->resourcePerformances.CIN.memoryUsage.createOperation;
            this->ram->addTask(byteCount);

            cMessage* timer = new cMessage("timer -> DB -> END CREATE CIN", PacketKind::TIMER);
            scheduleAt(simTime()+endProcessing, timer);
        } break;

        case ResourceType::SUB : {

        } break;

        case ResourceType::ACP : {

        } break;
    }
}
void DataStorage::scheduleRetrieveResource(DBMessage* dbPkt){

}
void DataStorage::scheduleUpdateResource(DBMessage* dbPkt){

}
void DataStorage::scheduleDeleteResource(DBMessage* dbPkt){

}
void DataStorage::handleStoragePacket(DBMessage* dbPkt){
    if (this->ram->isAtFullCapacity)
        this->waiting->insert(dbPkt);
    else
        switch(dbPkt->getOperation()){
            case Operation::_CREATE:
                scheduleCreateResource(dbPkt);
            break;

            case Operation::_RETRIEVE:
                scheduleRetrieveResource(dbPkt);
            break;

            case Operation::_UPDATE:
                scheduleUpdateResource(dbPkt);
            break;

            case Operation::_DELETE:
                scheduleDeleteResource(dbPkt);
            break;
        }
}

void DataStorage::handleTimer(cMessage* timer){
    delete timer;

    DBMessage* dbPkt = (DBMessage*)this->processing->pop();
    int byteCount = getByteCount(this->perf, dbPkt->getOperation(), dbPkt->getResourceType());
    this->ram->removeTask(byteCount);

    /*ServiceLayerControlInfo* oldCtrl = (ServiceLayerControlInfo*)dbPkt->removeControlInfo();
    ServiceLayerControlInfo* newCtrl = new ServiceLayerControlInfo(
            oldCtrl->ipDstAddress,
            oldCtrl->ipSrcAddress,
            oldCtrl->protocol);
    dbPkt->setControlInfo((cObject*)newCtrl);*/
    if (dbPkt->getKind() == PacketKind::REMOTE_DB) {
        const char* tmp = dbPkt->getSrcAddress();
        dbPkt->setSrcAddress(dbPkt->getDestAddress());
        dbPkt->setDestAddress(tmp);
    }
    send(dbPkt, "dbLink$o");
}


void DataStorage::handleMessage(cMessage *msg)
{
    switch(msg->getKind()) {
        case PacketKind::TIMER : {
            handleTimer(msg);
        } break;

        case PacketKind::LOCAL_DB :
        case PacketKind::REMOTE_DB : {
            DBMessage* dbPkt = check_and_cast<DBMessage*>(msg);
            handleStoragePacket(dbPkt);
        } break;
    }
}
