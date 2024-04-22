// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "Sensor.h"

Define_Module(Sensor);

void Sensor::initialize()
{
    name = par("name").stdstringValue();
    protocol = Protocol::StrToInt(getParentModule()->par("protocol").stdstringValue());

    json objParam = SensorDeserializer::parseInput(par("cinGenerator").stdstringValue());
    cinGenerator = SensorDeserializer::deserializeEventGenerator(objParam);
    cinGenerator->initialize(this->getRNG(0));

    Event* event = cinGenerator->getNextEvent();
    cMessage *timer = new cMessage("timer");
    timer->addPar("TimeSpan").setDoubleValue(event->TimeSpan);
    timer->addPar("DataSize").setDoubleValue(event->DataSize);
    scheduleAt(simTime()+event->TimeSpan, timer);


    this->cinRTT.setName(string(this->name + " - RTT").c_str());
}
void Sensor::runInitialProcedure(){
}

void Sensor::handleMessage(cMessage *msg)
{
    if (msg->isName("timer"))
    {
        OneM2MPrimitive* request = new OneM2MPrimitive("CREATE CIN", PacketKind::ONEM2M);
        request->setIsRequest(true);
        request->setOperation(Operation::_CREATE);
        char cinName[10];
        sprintf(cinName, "cin-%05d", this->cinID++);
        request->setMessageID(this->cinID);
        request->setResourceName(cinName);
        request->setResourceType(ResourceType::CIN);
        request->setUri(string("mn-cse/"+name+"/DATA").c_str());

        request->setSource(this->name.c_str());
        request->setDestination(getParentModule()->par("remoteCSE").stringValue());

        request->setFrom(this->name.c_str());
        request->setTo(getParentModule()->par("remoteCSE").stringValue());

        request->setContent("Sample Data");
        request->setByteLength(int(msg->par("DataSize").doubleValue()));

        request->setSrcAddress("");
        request->setDestAddress(getParentModule()->par("remoteCSEAddress").stringValue());
        request->setProtocol(this->protocol);

        send(request, gate("downLink$o"));
        sendingTime.insert(pair<int, double>(request->getMessageID(), simTime().dbl()));

        Event* event = cinGenerator->getNextEvent();
        msg->addPar("TimeSpan").setDoubleValue(event->TimeSpan);
        msg->addPar("DataSize").setDoubleValue(event->DataSize);
        scheduleAt(simTime()+event->TimeSpan, msg);
    }
    else {
        OneM2MPrimitive* response = check_and_cast<OneM2MPrimitive*>(msg);
        int id = response->getMessageID();
        double sendDate = sendingTime.at(id);
        double now = simTime().dbl();
        sendingTime.erase(id);
        double rtt =  now - sendDate;
        this->cinRTT.recordWithTimestamp(now, rtt);
        delete response;
    }
}
