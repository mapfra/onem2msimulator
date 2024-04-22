// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef LIB_MESSAGING_HPP_
#define LIB_MESSAGING_HPP_

#include <omnetpp.h>
using namespace omnetpp;
using namespace std;


class ServiceLayerControlInfo : cObject  {

  public:
    string ipSrcAddress;
    string ipDstAddress;
    int protocol;

    ServiceLayerControlInfo(string src, string dst, int proto) {
        this->ipSrcAddress = src;
        this->ipDstAddress = dst;
        this->protocol = proto;
    }
};

class DataBaseControlInfo : cObject {

  public:
    int resourceType;
    int operationType;

    DataBaseControlInfo(int resource, int operation) {
      this->resourceType = resource;
      this->operationType = operation;
    }
};




#endif /* LIB_MESSAGING_HPP_ */
