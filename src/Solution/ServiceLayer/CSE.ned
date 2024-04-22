// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.ServiceLayer;

import oneM2M.Solution.ServiceLayer.Components.Communication.COAPBinding;
import oneM2M.Solution.ServiceLayer.Components.Communication.HTTPBinding;
import oneM2M.Solution.ServiceLayer.Components.Communication.MQTTBinding;
import oneM2M.Solution.ServiceLayer.Components.Communication.MQTTBrokerService;
import oneM2M.Solution.ServiceLayer.Components.Communication.TCPBinding;
import oneM2M.Solution.ServiceLayer.Components.Persistence.DataStorage;
import oneM2M.Solution.ServiceLayer.Components.Core;
import oneM2M.Solution.ServiceLayer.Components.CSERouter;


module CSE
{
    parameters:
        @display("bgb=276.92,222.74;i=block/cogwheel;is=l");
        bool hasHTTP = default(true);
        bool hasCOAP = default(false);
        bool hasMQTT = default(false);
        
        bool hasLocalDB = default(true);
        string dbServerAddress = default("127.0.0.1");
        
        string performanceDescriptor = default("");
        
        string name = default(""); 
        string remoteCSE = default(""); 
        string remoteCSEAddress = default(""); 


    gates:
        inout downLink;

    submodules:
        core: Core {
            @display("p=139.32,29.240002;i=block/circle");
            performanceDescriptor = parent.performanceDescriptor;
            name = parent.name;
    		hasHTTP = parent.hasHTTP;
        	hasCOAP = parent.hasCOAP;
        	hasMQTT = parent.hasMQTT;        
        	hasLocalDB = parent.hasLocalDB;
        	dbServerAddress = parent.dbServerAddress; 
        	remoteCSE = parent.remoteCSE; 
        	remoteCSEAddress = parent.remoteCSEAddress;
        }
        coap: COAPBinding if hasCOAP {
            @display("p=102.340004,95.46001");
        }
        mqtt: MQTTBinding if hasMQTT {
            @display("p=26.660002,95.46001");
        }
        remoteDB: TCPBinding if !hasLocalDB {
            @display("p=240.8,95.46001");
        }
        localDB: DataStorage if hasLocalDB {
            @display("p=241.66,29.240002");
        }
        rt: CSERouter {
            @display("p=139.32,182.32");
        }
        http: HTTPBinding if hasHTTP {
            @display("p=170.28,95.46001");
        }
    connections allowunconnected:
        core.coapLink <--> coap.coreLink if hasCOAP;
        core.httpLink <--> http.coreLink if hasHTTP;
        core.mqttLink <--> mqtt.coreLink if hasMQTT;

        core.storageLink <--> localDB.dbLink if hasLocalDB;
        core.storageLink <--> remoteDB.coreLink if !hasLocalDB;

        rt.httpLink <--> http.routerLink if hasHTTP;
        rt.mqttLink <--> mqtt.routerLink if hasMQTT;
        rt.coapLink <--> coap.routerLink if hasCOAP;
        rt.dbLink <--> remoteDB.routerLink if !hasLocalDB;
        
        rt.downLink <--> downLink;

}
