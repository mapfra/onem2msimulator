// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "TCPBinding.h"

Define_Module(TCPBinding);

void TCPBinding::initialize()
{

}

void TCPBinding::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("routerLink$i")) {
        send(msg, "coreLink$o");
    }

    if (msg->arrivedOn("coreLink$i")) {
        send(msg, "routerLink$o");
    }
}
