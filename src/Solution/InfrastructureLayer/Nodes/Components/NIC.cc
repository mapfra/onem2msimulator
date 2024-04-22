// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "NIC.h"

Define_Module(NIC);

void NIC::initialize()
{

}

void NIC::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("upLink$i"))
        send(msg, "downLink$o");

    if (msg->arrivedOn("downLink$i"))
        send(msg, "upLink$o");
}
