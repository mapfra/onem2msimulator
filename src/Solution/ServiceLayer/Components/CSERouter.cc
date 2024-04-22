// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "CSERouter.h"


Define_Module(CSERouter);

void CSERouter::initialize() {
    this->hasCOAP = getParentModule()->par("hasCOAP").boolValue();
    this->hasHTTP = getParentModule()->par("hasHTTP").boolValue();
    this->hasMQTT = getParentModule()->par("hasMQTT").boolValue();
    this->hasLocalDB = getParentModule()->par("hasLocalDB").boolValue();
}

void CSERouter::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("httpLink$i")
     || msg->arrivedOn("coapLink$i")
     || msg->arrivedOn("mqttLink$i")
     || msg->arrivedOn("dbLink$i")){
        send(msg, "downLink$o");
    }
    else if (msg->arrivedOn("downLink$i")) {

        switch(msg->getKind()) {
            case PacketKind::ONEM2M: {
                OneM2MPrimitive* pkt = check_and_cast<OneM2MPrimitive*>(msg);
                int protocol = pkt->getProtocol();
                switch(protocol){
                    case Protocol::HTTP: {
                        send(pkt, "httpLink$o");
                    }
                    break;
                    case Protocol::COAP: {
                        send(pkt, "mqttLink$o");
                    }
                    break;
                    case Protocol::MQTT: {
                        send(pkt, "coapLink$o");
                    }
                    break;
                }
            } break;

            case PacketKind::REMOTE_DB: {
                DBMessage* dbPkt = check_and_cast<DBMessage*>(msg);
                send(dbPkt, "dbLink$o");
            } break;
        }
    }
}
