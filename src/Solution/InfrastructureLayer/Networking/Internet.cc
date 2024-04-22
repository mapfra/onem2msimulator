// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "Internet.h"

Define_Module(Internet);

void Internet::initialize()
{
    this->gateIndex = new map<string, int>();

    int linkCount = gateSize("link$i");
    for (int i=0; i<linkCount; i++) {
        cGate* prevGate = gate("link$i", i)->getPreviousGate();
        cModule* networkElement = prevGate->getOwnerModule();
        string subnet = networkElement->par("subnetAddress").stdstringValue();
        this->gateIndex->insert(pair<string, int>(subnet, i));
        EV << subnet << " at gate idx = " << i << '\n';
        cout << subnet << " at gate idx = " << i << '\n';
    }
}

void Internet::handleMessage(cMessage *msg)
{

}
