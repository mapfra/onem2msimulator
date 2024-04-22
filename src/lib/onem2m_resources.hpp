// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


#ifndef COMMON_ONEM2M_RESOURCES_H_
#define COMMON_ONEM2M_RESOURCES_H_

#include <omnetpp.h>
#include <map>

using namespace std;

class Resource {
  public:
    string name;
    string content;
    map<string, Resource*>* childResources;
    Resource* parentResource;

    Resource(string aName){
        this->name = aName;
        this->childResources = new map<string, Resource*>();
    }
    virtual ~Resource(){
        delete this->childResources;
    }
    void setParentResource(Resource* parent){
        this->parentResource = parent;
    }
    void addChildResource(Resource* child){
        if (this->childResources->find(child->name) == this->childResources->end()) {
            child->setParentResource(this);
            this->childResources->insert(pair<string, Resource*>(child->name, child));
        }
    }
    Resource* element(string name){
        Resource* result = NULL;
        map<string, Resource*>::iterator it;
        it = this->childResources->find(name);
        if (it != childResources->end())
            result = it->second;
        return result;
    }
};
class AccessControlPolicy: public Resource {
public:
    AccessControlPolicy(string aName): Resource (aName) {}
    virtual ~AccessControlPolicy(){}
};
class ApplicationEntity: public Resource {
public:
    ApplicationEntity(string aName): Resource (aName) {}
    virtual ~ApplicationEntity(){}
};
class Container: public Resource {
public:
    Container(string aName): Resource (aName) {}
    virtual ~Container(){}
};
class ContentInstance: public Resource {
public:
    ContentInstance(string aName): Resource (aName) {}
    virtual ~ContentInstance(){}
};
class CSEBase: public Resource {
public:
    CSEBase(string aName): Resource (aName) {}
    virtual ~CSEBase(){}
};
class LatestOldest: public Resource {
public:
    LatestOldest(string aName): Resource (aName) {}
    virtual ~LatestOldest(){}
};
class RemoteCSE: public Resource {
public:
    RemoteCSE(string aName): Resource (aName) {}
    virtual ~RemoteCSE(){}
};
class Subscription: public Resource {
public:
    Subscription(string aName): Resource (aName) {}
    virtual ~Subscription(){}
};

#endif /* COMMON_ONEM2M_RESOURCES_H_ */
