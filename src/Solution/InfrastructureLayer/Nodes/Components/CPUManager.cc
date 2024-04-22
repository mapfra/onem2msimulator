// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#include "CPUManager.h"

Define_Module(CPUManager);



void CPUManager::initialize()
{
    this->IPS = par("IPS").intValue();
    this->nextAvailablity = 0;
}

double CPUManager::addTask(int instrCount){

    double duration = (double) instrCount / this->IPS;
    this->nextAvailablity += duration;
    return this->nextAvailablity;
}

double CPUManager::getProcessingTime(int instrCount){
    return (double)instrCount / this->IPS;
}

