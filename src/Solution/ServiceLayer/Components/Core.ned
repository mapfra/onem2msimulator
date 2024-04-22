// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com
 

package oneM2M.Solution.ServiceLayer.Components;

//
// TODO auto-generated module
//
simple Core
{
    parameters:
    	@display("i=block/circle");
    	string name;
    	string onem2mReleaseVersion = default("V1");
    	string performanceDescriptor;
    	
    	bool hasHTTP;
        bool hasCOAP;
        bool hasMQTT;        

        bool hasLocalDB;
        string dbServerAddress; 

        string remoteCSE; 
        string remoteCSEAddress;

    gates:        
        inout httpLink;
        inout coapLink;
        inout mqttLink;
        inout storageLink;
}
