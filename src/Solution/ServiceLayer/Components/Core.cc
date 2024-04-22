// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "Core.h"

Define_Module(Core);


vector<string> split (const string &str, char delim) {
    vector<string> result;
    stringstream strStream (str);
    string item;
    while (getline (strStream, item, delim))
        result.push_back (item);
    return result;
}


void Core::initialize()
{
    // core > CSE > CSENode > *Manager
    this->cpu = check_and_cast<CPUManager*>(getParentModule()->getParentModule()->getSubmodule("cpuManager"));
    this->ram = check_and_cast<RAMManager*>(getParentModule()->getParentModule()->getSubmodule("ramManager"));
    this->io = check_and_cast<IOManager*>(getParentModule()->getParentModule()->getSubmodule("ioManager"));

    string perfDescriptorString = par("performanceDescriptor");
    json jsonObj = PerformanceDescriptorDeserializer::parseInput(perfDescriptorString);
    this->perf = PerformanceDescriptorDeserializer::getPerformanceDescriptor(jsonObj);

    this->cseName = par("name").stdstringValue();
    this->hasHTTP = par("hasHTTP").boolValue();
    this->hasCOAP = par("hasCOAP").boolValue();
    this->hasMQTT = par("hasMQTT").boolValue();
    this->hasLocalDB = par("hasLocalDB").boolValue();
    this->dbServerAddress = par("dbServerAddress").stdstringValue();
    this->remoteCSE = par("remoteCSE").stdstringValue();
    this->remoteCSEAddress = par("remoteCSEAddress").stdstringValue();

    this->resourceTree = new CSEBase(this->cseName);
    this->resourceTree->addChildResource(new RemoteCSE(this->remoteCSE));

    this->waiting = new cPacketQueue();
    this->processing = new cPacketQueue();


    this->processingTime.setName(string(this->cseName + " - ProcessingTime").c_str());
}



void Core::handleTimer(cMessage* timer){
    delete timer;

    OneM2MPrimitive* pkt = (OneM2MPrimitive*)this->processing->pop();
    int byteCount = getByteCount(this->perf, pkt->getOperation(), pkt->getResourceType());
    this->ram->removeTask(byteCount);

    /*const char* initialSource = pkt->getSource();
    pkt->setSource(this->cseName.c_str());
    pkt->setDestination(initialSource);*/

    switch(pkt->getOperation()){
        case Operation::_CREATE:{
            createResource(pkt);
            pkt->setStatus(ResponseStatusCode::CREATED);
        }
        break;

        case Operation::_RETRIEVE:{
            retrieveResource(pkt);
            pkt->setStatus(ResponseStatusCode::OK);
        }
        break;

        case Operation::_UPDATE:{
            updateResource(pkt);
            pkt->setStatus(ResponseStatusCode::UPDATED);
        }
        break;

        case Operation::_DELETE:{
            deleteResource(pkt);
            pkt->setStatus(ResponseStatusCode::DELETED);
        }
        break;
    }

    DBMessage* dbPkt = new DBMessage("dbMessage");
    dbPkt->setOperation(pkt->getOperation());
    dbPkt->setResourceType(pkt->getResourceType());
    dbPkt->encapsulate(pkt);
    if (this->hasLocalDB) {
        dbPkt->setKind(PacketKind::LOCAL_DB);
        dbPkt->setSrcAddress("127.0.0.1");
        dbPkt->setDestAddress("127.0.0.1");
        dbPkt->setProtocol(Protocol::DB);
    }
    else {
        dbPkt->setKind(PacketKind::REMOTE_DB);
        dbPkt->setSrcAddress("");
        dbPkt->setDestAddress(this->dbServerAddress.c_str());
        dbPkt->setProtocol(Protocol::TCP);
    }
    send(dbPkt, "storageLink$o");
}

void Core::createResource(OneM2MPrimitive* pkt){
    string uri = string(pkt->getUri());
    vector<string> parts = split(uri, '/');

    switch (pkt->getResourceType()){
        case ResourceType::AE : {
            ApplicationEntity *ae = new ApplicationEntity(string(pkt->getResourceName()));
            ae->content = string(pkt->getContent());
            this->resourceTree->addChildResource(ae);
        }
        break;


        case ResourceType::CNT : {
            Container *cnt = new Container(string(pkt->getResourceName()));
            cnt->content = string(pkt->getContent());
            string aeName = parts[1];

            if (this->resourceTree->element(aeName) == NULL)
                this->resourceTree->addChildResource(new ApplicationEntity(aeName));

            this->resourceTree->element(aeName)->addChildResource(cnt);
        }
        break;

        case ResourceType::CIN : {
            ContentInstance *cin = new ContentInstance(string(pkt->getResourceName()));
            cin->content = string(pkt->getContent());
            string aeName = parts[1];
            string cntName = parts[2];

            if (this->resourceTree->element(aeName) == NULL)
                this->resourceTree->addChildResource(new ApplicationEntity(aeName));

            if (this->resourceTree->element(aeName)->element(cntName) == NULL)
                this->resourceTree->element(aeName)->addChildResource(new Container(cntName));

            this->resourceTree->element(aeName)->element(cntName)->addChildResource(cin);
        }
        break;

        case ResourceType::SUB : {
            Subscription *sub = new Subscription(string(pkt->getResourceName()));
            sub->content = string(pkt->getContent());
            string aeName = parts[1];
            string cntName = parts[2];

            if (this->resourceTree->element(aeName) == NULL)
                this->resourceTree->addChildResource(new ApplicationEntity(aeName));

            if (this->resourceTree->element(aeName)->element(cntName) == NULL)
                this->resourceTree->element(aeName)->addChildResource(new Container(cntName));

            this->resourceTree->element(aeName)->element(cntName)->addChildResource(sub);
        }
        break;

        case ResourceType::ACP: {
            AccessControlPolicy *acp = new AccessControlPolicy(string(pkt->getResourceName()));
            acp->content = string(pkt->getContent());
            this->resourceTree->addChildResource(acp);
        }
        break;
    }
}
void Core::retrieveResource(OneM2MPrimitive* pkt){
}
void Core::updateResource(OneM2MPrimitive* pkt){
}
void Core::deleteResource(OneM2MPrimitive* pkt){
}


void Core::scheduleCreateResource(OneM2MPrimitive* pkt){

    switch (pkt->getResourceType()){
        case ResourceType::AE: {
            this->processing->insert(pkt);

            int instrCount = this->perf->resourcePerformances.AE.processorUsage.createOperation;
            double procTime = this->cpu->getProcessingTime(instrCount);
            double endProcessing = this->cpu->nextAvailablity + procTime;
            this->cpu->addTask(instrCount);

            int byteCount = this->perf->resourcePerformances.AE.memoryUsage.createOperation;
            this->ram->addTask(byteCount);

            cMessage* timer = new cMessage("timer -> CSE -> END CREATE AE", PacketKind::TIMER);
            scheduleAt(simTime()+endProcessing, timer);
        } break;

        case ResourceType::CNT : {

        } break;

        case ResourceType::CIN : {
            this->processing->insert(pkt);

            int instrCount = this->perf->resourcePerformances.CIN.processorUsage.createOperation;
            double procTime = this->cpu->getProcessingTime(instrCount);
            double endProcessing = this->cpu->nextAvailablity + procTime;
            this->cpu->addTask(instrCount);

            int byteCount = this->perf->resourcePerformances.CIN.memoryUsage.createOperation;
            this->ram->addTask(byteCount);

            cMessage* timer = new cMessage("timer -> CSE -> END CREATE CIN", PacketKind::TIMER);
            scheduleAt(simTime()+endProcessing, timer);
        } break;

        case ResourceType::SUB : {

        } break;

        case ResourceType::ACP : {

        } break;
    }
}
void Core::scheduleRetrieveResource(OneM2MPrimitive* pkt){

}
void Core::scheduleUpdateResource(OneM2MPrimitive* pkt){

}
void Core::scheduleDeleteResource(OneM2MPrimitive* pkt){

}
void Core::handleOneM2MPacket(OneM2MPrimitive* pkt){
    if (strcmp(pkt->getDestination(), this->cseName.c_str()) == 0) {
        if (this->ram->isAtFullCapacity) {
            this->waiting->insert(pkt);
        }
        else {
            if (pkt->isRequest()){
                switch(pkt->getOperation()){
                    case Operation::_CREATE:
                        scheduleCreateResource(pkt);
                    break;

                    case Operation::_RETRIEVE:
                        scheduleRetrieveResource(pkt);
                    break;

                    case Operation::_UPDATE:
                        scheduleUpdateResource(pkt);
                    break;

                    case Operation::_DELETE:
                        scheduleDeleteResource(pkt);
                    break;
                }
            }
        }
    }
    else {
        // forward
    }
}

void Core::handleStoragePacket(DBMessage* dbPkt){
    OneM2MPrimitive* pkt = check_and_cast<OneM2MPrimitive*>(dbPkt->decapsulate());
    delete dbPkt;

    pkt->setIsRequest(false);

    string tmp = string(pkt->getSrcAddress());
    pkt->setSrcAddress(pkt->getDestAddress());
    pkt->setDestAddress(tmp.c_str());

    tmp = string(pkt->getSource());
    pkt->setSource(pkt->getDestination());
    pkt->setDestination(tmp.c_str());

    tmp = string(pkt->getFrom());
    pkt->setFrom(pkt->getTo());
    pkt->setTo(tmp.c_str());

    switch(pkt->getProtocol()){
        case Protocol::HTTP: {
            send(pkt, "httpLink$o");
        } break;

        case Protocol::MQTT: {
            send(pkt, "mqttLink$o");
        } break;

        case Protocol::COAP: {
            send(pkt, "coapLink$o");
        } break;
    }

    int id = pkt->getMessageID();
    double receptionTime = receivedDate.at(id);
    receivedDate.erase(id);
    double now = simTime().dbl();
    double processTime = now - receptionTime;
    this->processingTime.recordWithTimestamp(now,  processTime);
}


void Core::handleMessage(cMessage *msg)
{
    switch(msg->getKind()) {
        case PacketKind::TIMER : {
            handleTimer(msg);
        } break;

        case PacketKind::LOCAL_DB : // This is a response from local DB
        case PacketKind::REMOTE_DB : {       // This is a response from remote DB
            DBMessage* dbPkt = check_and_cast<DBMessage*>(msg);
            handleStoragePacket(dbPkt);
        } break;

        case PacketKind::ONEM2M : {        // this is a new packet received over the network
            OneM2MPrimitive* packet = check_and_cast<OneM2MPrimitive*>(msg);
            handleOneM2MPacket(packet);
            this->receivedDate.insert(pair<int, double>(packet->getMessageID(), simTime().dbl()));
        } break;
    }
}
