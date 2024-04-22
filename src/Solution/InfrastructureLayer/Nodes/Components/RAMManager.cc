// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "RAMManager.h"

Define_Module(RAMManager);

void RAMManager::addTask(int bytes){
    this->currentBytes += bytes;
    if (this->currentBytes > this->maximumBytes){
        this->currentBytes = this->maximumBytes;
        this->isAtFullCapacity = true;
    }


    this->memoryUsage.recordWithTimestamp(simTime(),  this->currentBytes );
}
void RAMManager::removeTask(int bytes){
    this->currentBytes -= bytes;

    if (this->currentBytes < this->maximumBytes){
        this->isAtFullCapacity = false;
    }

    if (this->currentBytes < 0){
        this->currentBytes = 0;
    }

    this->memoryUsage.recordWithTimestamp(simTime(),  this->currentBytes );
}

void RAMManager::initialize()
{
    this->currentBytes = 0;
    this->isAtFullCapacity = false;
    this->maximumBytes = par("maximumBytes").intValue();

    string hwNodeName = getParentModule()->getName();
    this->memoryUsage.setName(string(hwNodeName + " - RAMManager").c_str());
    this->memoryUsage.recordWithTimestamp(simTime(),  this->currentBytes );
}
