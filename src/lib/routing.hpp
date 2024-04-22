// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef LIB_ROUTING_HPP_
#define LIB_ROUTING_HPP_

#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

class RoutingTools {
  public:
    static int ipStrToInt(string ip) {
        int res = 0;
        size_t pos = 0;
        string component;
        ip += ".";
        while ((pos = ip.find(".")) != string::npos) {
            component = ip.substr(0, pos);
            ip.erase(0, pos + 1);
            res <<= 8;
            res += atoi(component.c_str());
        }
        return res;
    }

    static bool isInSameNetwork(string host, string net) {
        int pos = net.find("/");
        string netAddress = net.substr(0,  pos);
        net.erase(0, pos+1);
        string netMask = net;

        int mask = 0xFFFFFFFF << (32 - atoi(netMask.c_str()));
        bool res = ((ipStrToInt(host) & mask) == (ipStrToInt(netAddress) & mask));
        return res;
    }

    static int getHostGateIndex(string dest, map<string, int> *table){
        int res = -1;
        for (map<string,int>::iterator it = table->begin(); it!=table->end(); ++it){
            cout << it->first << " => " << it->second << '\n';
            if (dest == it->first)
                res = it->second;
        }
        return res;
    }

    static int getNetworkGateIndex(string dest, map<string, int> *table){
        int res = -1;
        for (map<string,int>::iterator it = table->begin(); it!=table->end(); ++it){
            cout << it->first << " => " << it->second << '\n';
            if (isInSameNetwork(dest, it->first))
                res = it->second;
        }
        return res;
    }
};
#endif /* LIB_ROUTING_HPP_ */
