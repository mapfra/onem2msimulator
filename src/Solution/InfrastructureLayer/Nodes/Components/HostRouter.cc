// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#include "HostRouter.h"


Define_Module(HostRouter);

const char MQTT_BROKER_NAME[] = "MQTTBrokerService";
const char CSE_NAME[] = "CSE";
const char DB_NAME[] = "DataStorage";

void HostRouter::initialize()
{

    this->upperGateIndex = new map<string, int>();
    int linkCount = gateSize("upLink$i");
    for (int i=0; i<linkCount; i++) {
        cGate* prevGate = gate("upLink$i", i)->getPreviousGate();
        cModule* upperModule = prevGate->getOwnerModule();
        const char* moduleType = upperModule->getModuleType()->getName();

        if (strcmp(moduleType, MQTT_BROKER_NAME) == 0){
            this->upperGateIndex->insert(pair<string, int>(MQTT_BROKER_NAME, i));
            EV << MQTT_BROKER_NAME << " at gate idx = " << i << '\n';
            cout << MQTT_BROKER_NAME << " at gate idx = " << i << '\n';
        }
        else if (strcmp(moduleType, CSE_NAME) == 0){
            const char* cseName = upperModule->par("name").stringValue();
            this->upperGateIndex->insert(pair<string, int>(cseName, i));
            EV << CSE_NAME << " at upper gate idx = " << i << '\n';
            cout << CSE_NAME << " at upper gate idx = " << i << '\n';
        }
        else if (strcmp(moduleType, DB_NAME) == 0){
            this->upperGateIndex->insert(pair<string, int>(DB_NAME, i));
            EV << DB_NAME << " at upper gate idx = " << i << '\n';
            cout << DB_NAME << " at upper gate idx = " << i << '\n';
        }
        else if (strcmp(moduleType, "Application") == 0) {
            string appName = upperModule->par("name").stdstringValue();

            // get the list of sensor belonging to this application
            int sensorCount = upperModule->getSubmoduleVectorSize("sensor");
            for (int k=0; k<sensorCount; k++){
                string sensorName = upperModule->getSubmodule("sensor", k)->par("name").stringValue();
                this->upperGateIndex->insert(pair<string, int>(sensorName, i));
                EV << appName <<">"<< sensorName << " at upper gate idx = " << i << '\n';
                cout << appName <<">"<< sensorName << " at upper gate idx = " << i << '\n';
            }

            // get the list of actuator belonging to this application
            int actuatorCount = upperModule->getSubmoduleVectorSize("actuator");
            for (int k=0; k<actuatorCount; k++){
                string actuatorName = upperModule->getSubmodule("actuator", k)->par("name").stringValue();
                this->upperGateIndex->insert(pair<string, int>(actuatorName, i));
                EV << appName <<">"<< actuatorName << " at upper gate idx = " << i << '\n';
                cout << appName <<">"<< actuatorName << " at upper gate idx = " << i << '\n';
            }
        }
    }

    this->lowerGateIndex = new map<string, int>();
    linkCount = gateSize("downLink$i");
    for (int i=0; i<linkCount; i++) {
        cGate* prevGate = gate("downLink$i", i)->getPreviousGate();
        cModule* nicModule = prevGate->getOwnerModule();
        string networkAddress = nicModule->par("networkAddress").stdstringValue();

        EV << networkAddress << " at lower gate idx = " << i << '\n';
        cout << networkAddress << " at lower gate idx = " << i << '\n';
    }
}

void HostRouter::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("upLink$i")){
        switch (msg->getKind()) {
            case PacketKind::ONEM2M : {
                OneM2MPrimitive* pkt = dynamic_cast<OneM2MPrimitive*>(msg);
                // To-Do : select the best NIC
                string srcIP = gate("downLink$o", 0)->getNextGate()->getOwnerModule()->par("networkAddress").stdstringValue();
                pkt->setSrcAddress(srcIP.c_str());

                NetworkPacket* netPkt = new NetworkPacket(pkt->getName(), PacketKind::NETWORK);
                netPkt->setSrcAddress(srcIP.c_str());
                netPkt->setDestAddress(pkt->getDestAddress());
                netPkt->setProtocol(pkt->getProtocol());
                netPkt->encapsulate(pkt);
                send(netPkt, "downLink$o", 0);
            } break;

            case PacketKind::REMOTE_DB : {
                DBMessage* dbPkt = dynamic_cast<DBMessage*>(msg);
                // To-Do : select the best NIC
                string srcIP = gate("downLink$o", 0)->getNextGate()->getOwnerModule()->par("networkAddress").stdstringValue();
                dbPkt->setSrcAddress(srcIP.c_str());

                NetworkPacket* netPkt = new NetworkPacket(dbPkt->getName(), PacketKind::NETWORK);
                netPkt->setSrcAddress(srcIP.c_str());
                netPkt->setDestAddress(dbPkt->getDestAddress());
                netPkt->setProtocol(dbPkt->getProtocol());
                netPkt->encapsulate(dbPkt);
                send(netPkt, "downLink$o", 0);
            } break;
        }
    }

    if (msg->arrivedOn("downLink$i")){
        NetworkPacket* netPkt = check_and_cast<NetworkPacket*>(msg);
        cPacket* payload = netPkt->decapsulate();

        switch (payload->getKind()){
            case PacketKind::ONEM2M: {
                OneM2MPrimitive* oneM2MMessage = check_and_cast<OneM2MPrimitive*>(payload);
                send(oneM2MMessage, "upLink$o", this->upperGateIndex->at(oneM2MMessage->getDestination()));
            } break;

            case PacketKind::REMOTE_DB: {
                DBMessage* dbMessage = check_and_cast<DBMessage*>(payload);
                send(dbMessage, "upLink$o", this->upperGateIndex->at(DB_NAME));
            }break;
        }
        delete netPkt;
    }
}
