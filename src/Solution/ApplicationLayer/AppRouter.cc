// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "AppRouter.h"

Define_Module(AppRouter);

void AppRouter::initialize()
{
    this->gateIndex = new map<string, int>();

    int linkCount = gateSize("upLink$i");
    for (int i=0; i<linkCount; i++) {
        cGate* prevGate = gate("upLink$i", i)->getPreviousGate();
        cModule* appModule = prevGate->getOwnerModule();
        string name = appModule->par("name").stdstringValue();
        this->gateIndex->insert(pair<string, int>(name, i));
        EV << name << " at gate idx i = " << i << '\n';
        cout << name << " at gate idx i = " << i << '\n';
    }
}

void AppRouter::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("upLink$i"))
        send(msg, "downLink$o");

    if (msg->arrivedOn("downLink$i")) {
        // identify target sensor/actuator
        OneM2MPrimitive* pkt = check_and_cast<OneM2MPrimitive*>(msg);
        string moduleName = string(pkt->getDestination());
        int gateIdx = this->gateIndex->find(moduleName)->second;
        send(msg, "upLink$o", gateIdx);
    }
}
