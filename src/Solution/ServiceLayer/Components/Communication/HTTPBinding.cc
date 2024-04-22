// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "HTTPBinding.h"

Define_Module(HTTPBinding);

void HTTPBinding::initialize()
{

}

void HTTPBinding::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("routerLink$i")) {
        send(msg, "coreLink$o");
    }

    if (msg->arrivedOn("coreLink$i")) {
        send(msg, "routerLink$o");
    }
}
