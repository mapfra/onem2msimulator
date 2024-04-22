// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <omnetpp.h>
using namespace std;

class ResourceType
{
  public:
    static const int ACP  = 1;
    static const int AE   = 2;
    static const int CNT  = 3;
    static const int CIN  = 4;
    static const int SUB  = 5;
};

class Operation
{
  public:
    static const int _CREATE   = 1;
    static const int _RETRIEVE = 2;
    static const int _UPDATE   = 3;
    static const int _DELETE   = 4;
    static const int _NOTIFY   = 5;
};

class ResponseStatusCode
{
  public:
    static const int OK       = 2000;
    static const int CREATED  = 2001;
    static const int DELETED  = 2002;
    static const int UPDATED  = 2004;
};


class Protocol
{
  public:
    static const int HTTP = 1;
    static const int COAP = 2;
    static const int MQTT = 3;
    static const int TCP  = 4;
    static const int DB   = 5;

    static int StrToInt(string protocol){
        if (protocol == "http")
            return HTTP;
        if (protocol == "coap")
            return COAP;
        if (protocol == "mqtt")
            return MQTT;
        if (protocol == "tcp")
            return TCP;
        if (protocol == "db")
            return DB;

        return -1;
    }
};

class PacketKind {
public:
  static const int ONEM2M    = 1;
  static const int LOCAL_DB  = 2;
  static const int REMOTE_DB = 3;
  static const int NETWORK   = 4;
  static const int TIMER     = 5;
};

#endif /* COMMON_CONSTANTS_HPP_ */
