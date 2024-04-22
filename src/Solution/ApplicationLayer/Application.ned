// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.ApplicationLayer;

import oneM2M.Solution.ApplicationLayer.Sensor;
import oneM2M.Solution.ApplicationLayer.Actuator;
import oneM2M.Solution.ApplicationLayer.AppRouter;

module Application
{
    parameters:
        int sensorCount;
        int actuatorCount;
        
        string name = default("");   
        string protocol = default("http"); 
        string remoteCSE;
        string remoteCSEAddress; 
        
        @display("i=block/app");
    gates:
        inout downLink;
    submodules:
        sensor[sensorCount]: Sensor {
            @display("p=210,43");
        }
        actuator[actuatorCount]: Actuator {
            @display("p=74,43;i=block/rx");
        }
        rt: AppRouter {
            @display("p=142,143");
        }
    connections:
        for i=0..sensorCount-1 {
            sensor[i].downLink <--> rt.upLink++;
        }
        for i=0..actuatorCount-1 {
            actuator[i].downLink <--> rt.upLink++;
        }
        downLink <--> rt.downLink;
}
