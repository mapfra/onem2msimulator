// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

#ifndef LIB_SERIALIZATION_HPP_
#define LIB_SERIALIZATION_HPP_

#include <omnetpp.h>
#include <fstream>

#include "constants.hpp"
#include "json.hpp"

using namespace omnetpp;
using namespace std;
using json = nlohmann::json;

class Distribution{
  public:
    cRNG* rng;
    virtual double getNextValue() = 0;
};
class Constant : public Distribution {
  public:
    double constant;
    double getNextValue(){
        return constant;
    }
};
class Exponential : public Distribution {
  public:
    double lambda;
    double getNextValue(){
        return exponential(rng, lambda);
    }
};
class Uniform : public Distribution {
  public:
    double lowerBound;
    double upperBound;
    double getNextValue(){
        return uniform(rng, lowerBound, upperBound);
    }
};

class Event
{
  public:
    double TimeSpan;
    unsigned int DataSize;
};
class EventGenerator
{
  public:
    virtual void initialize(cRNG* aRNG) = 0;
    virtual Event* getNextEvent() = 0;
};
class SimulatedEvent : public EventGenerator
{
  public:
    Distribution *eventDistribution;
    Distribution *dataSizeDistribution;
    void initialize(cRNG* aRNG) {
        eventDistribution->rng = aRNG;
        dataSizeDistribution->rng = aRNG;
    }
    Event* getNextEvent(){
        Event *evt = new Event();
        evt->TimeSpan =eventDistribution->getNextValue();
        evt->DataSize = dataSizeDistribution->getNextValue();
        return evt;
    }
};
class RecordedEvent : public EventGenerator
{
  private:
    list<Event> events;
    list<Event>::iterator it;
    void loadEventsFromFile(const string & filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)){
            istringstream stringStream(line);
            double timeSpan;
            unsigned int dataSize;
            if (!(stringStream >> timeSpan >> dataSize))
                throw runtime_error("Error while reading file !");
            Event event;
            event.TimeSpan = timeSpan;
            event.DataSize = dataSize;
            events.push_back(event);
        }
    }
  public:
    string fileName;
    void initialize(cRNG* aRNG) {
        loadEventsFromFile(fileName);
        it = events.begin();
    }
    Event* getNextEvent(){
        if (it == events.end())
            it = events.begin();

        Event *evt = it.operator ->();
        ++it;
        return evt;
    }
};

class SensorDeserializer{
  public:

    static json parseInput(const std::string& input) {
        string prefix = input.substr(0, 5);
        if (prefix == "data:") {
            string jsonString = input.substr(7);
            return json::parse(jsonString);
        } else if (prefix == "file:") {
            string filename = input.substr(7);
            ifstream inputFileStream(filename);
            json jsonObject;
            inputFileStream >> jsonObject;
            return jsonObject;
        } else {
            throw runtime_error("unknown prefix in input string !");
        }
    }

    static Distribution* deserializeDistribution(const json& jsonObject) {
        string type = jsonObject["type"];
        if (type == "Constant") {
            Constant* dist = new Constant();
            dist->constant = jsonObject["constant"];
            return dist;
        } else if (type == "Exponential") {
            Exponential* dist = new Exponential();
            dist->lambda = jsonObject["lambda"];
            return dist;
        } else if (type == "Uniform") {
            Uniform* dist = new Uniform();
            dist->lowerBound = jsonObject["lowerBound"];
            dist->upperBound = jsonObject["upperBound"];
            return dist;
        } else {
            throw runtime_error("Unknown type : " + type);
        }
    }
    static EventGenerator* deserializeEventGenerator(const json& jsonObject) {
        std::string type = jsonObject["type"];
        if (type == "SimulatedEvent") {
            SimulatedEvent* simEvent = new SimulatedEvent();
            simEvent->eventDistribution = SensorDeserializer::deserializeDistribution(jsonObject["eventDistribution"]);
            simEvent->dataSizeDistribution = SensorDeserializer::deserializeDistribution(jsonObject["dataSizeDistribution"]);
            return simEvent;
        } else if (type == "RecordedEvent") {
            RecordedEvent* recEvent = new RecordedEvent();
            recEvent->fileName = jsonObject["fileName"];
            return recEvent;
        } else {
            throw runtime_error("Unknown type : " + type);
        }
    }
};


struct Performance {
    int createOperation;
    int retreiveOperation;
    int updateOperation;
    int deleteOperation;
};

struct ResourcePerformance {
    Performance processorUsage;
    Performance memoryUsage;
    Performance diskUsage;
};

struct ResourcePerformances {
    ResourcePerformance AE;
    ResourcePerformance CNT;
    ResourcePerformance CIN;
    ResourcePerformance LATEST;
    ResourcePerformance ACP;
    ResourcePerformance SUB;
};

struct PerformanceDescriptor {
    std::string name;
    std::string product;
    std::string version;
    int minProcessorUsage;
    int minMemoryUsage;
    ResourcePerformances resourcePerformances;
};

class PerformanceDescriptorDeserializer {
  public:
    static json parseInput(const std::string& input){
        string prefix = input.substr(0, 5);
        if (prefix == "data:") {
            string jsonString = input.substr(7);
            return json::parse(jsonString);
        } else if (prefix == "file:") {
            string filename = input.substr(7);
            ifstream inputFileStream(filename);
            json jsonObject;
            inputFileStream >> jsonObject;
            return jsonObject;
        } else {
            throw runtime_error("unknown prefix in input string !");
        }
    }

    static Performance getPerformance(const json obj) {
        Performance performance;
        performance.createOperation = obj.at("createOperation").get<int>();
        performance.retreiveOperation = obj.at("retreiveOperation").get<int>();
        performance.updateOperation = obj.at("updateOperation").get<int>();
        performance.deleteOperation = obj.at("deleteOperation").get<int>();
        return performance;
    }
    static ResourcePerformance getResourcePerformance(const json obj) {
        ResourcePerformance resourcePerformance;
        resourcePerformance.processorUsage = getPerformance(obj.at("processorUsage"));
        resourcePerformance.memoryUsage = getPerformance(obj.at("memoryUsage"));
        resourcePerformance.diskUsage = getPerformance(obj.at("diskUsage"));
        return resourcePerformance;
    }
    static ResourcePerformances getResourcePerformances(const json obj) {
        ResourcePerformances resourcePerformances;
        resourcePerformances.AE = getResourcePerformance(obj.at("AE"));
        resourcePerformances.CNT = getResourcePerformance(obj.at("CNT"));
        resourcePerformances.CIN = getResourcePerformance(obj.at("CIN"));
        resourcePerformances.LATEST = getResourcePerformance(obj.at("LATEST"));
        resourcePerformances.ACP = getResourcePerformance(obj.at("ACP"));
        resourcePerformances.SUB = getResourcePerformance(obj.at("SUB"));
        return resourcePerformances;
    }
    static PerformanceDescriptor* getPerformanceDescriptor(const json obj){
        PerformanceDescriptor* performanceDescriptor = new PerformanceDescriptor();
        json innerObject = obj.at("PerformanceDescriptor");
        performanceDescriptor->name = innerObject.at("name").get<string>();
        performanceDescriptor->product = innerObject.at("product").get<string>();
        performanceDescriptor->version = innerObject.at("version").get<string>();
        performanceDescriptor->minProcessorUsage = innerObject.at("minProcessorUsage").get<int>();
        performanceDescriptor->minMemoryUsage = innerObject.at("minMemoryUsage").get<int>();
        performanceDescriptor->resourcePerformances = getResourcePerformances(innerObject.at("resourcePerformances"));

        return performanceDescriptor;
    }


};


inline int getInstructionCount(PerformanceDescriptor* perf, int operationType, int resourceType ){
    Performance* cpuPerformance;
    int result = -1;
    switch (resourceType){
        case ResourceType::AE:
            cpuPerformance = &(perf->resourcePerformances.AE.processorUsage);
        break;

        case ResourceType::CNT:
            cpuPerformance = &(perf->resourcePerformances.CNT.processorUsage);
        break;

        case ResourceType::CIN:
            cpuPerformance = &(perf->resourcePerformances.CIN.processorUsage);
        break;

        case ResourceType::SUB:
            cpuPerformance = &(perf->resourcePerformances.SUB.processorUsage);
        break;

        case ResourceType::ACP:
            cpuPerformance = &(perf->resourcePerformances.ACP.processorUsage);
        break;
    }

    switch(operationType){
        case Operation::_CREATE:
            result = cpuPerformance->createOperation;
        break;

        case Operation::_RETRIEVE:
            result = cpuPerformance->retreiveOperation;
        break;

        case Operation::_UPDATE:
            result = cpuPerformance->updateOperation;
        break;

        case Operation::_DELETE:
            result = cpuPerformance->deleteOperation;
        break;
    }

    return result;
}
inline int getByteCount(PerformanceDescriptor* perf, int operationType, int resourceType){
    Performance* ramPerformance;
    int result = -1;
    switch (resourceType){
        case ResourceType::AE:
            ramPerformance = &(perf->resourcePerformances.AE.memoryUsage);
        break;

        case ResourceType::CNT:
            ramPerformance = &(perf->resourcePerformances.CNT.memoryUsage);
        break;

        case ResourceType::CIN:
            ramPerformance = &(perf->resourcePerformances.CIN.memoryUsage);
        break;

        case ResourceType::SUB:
            ramPerformance = &(perf->resourcePerformances.SUB.memoryUsage);
        break;

        case ResourceType::ACP:
            ramPerformance = &(perf->resourcePerformances.ACP.memoryUsage);
        break;
    }

    switch(operationType){
        case Operation::_CREATE:
            result = ramPerformance->createOperation;
        break;

        case Operation::_RETRIEVE:
            result = ramPerformance->retreiveOperation;
        break;

        case Operation::_UPDATE:
            result = ramPerformance->updateOperation;
        break;

        case Operation::_DELETE:
            result = ramPerformance->deleteOperation;
        break;
    }

    return result;
}


#endif /* LIB_SERIALIZATION_HPP_ */

