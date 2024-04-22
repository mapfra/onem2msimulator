// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com
 

package oneM2M.Solution.InfrastructureLayer.Nodes;

import oneM2M.Solution.InfrastructureLayer.Nodes.Components.NIC;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.HostRouter;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.CPUManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.IOManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.RAMManager;

import oneM2M.Solution.ApplicationLayer.Application;
import oneM2M.Solution.ServiceLayer.CSE;


module HWNode
{
    parameters:
        string id = default("");

        string name = default("");
        string location = default("");
        int processor = default(100);
        int memory = default(100);
        int disk = default(100);

        int nicCount = default(1);
        int appCount = default(0);

        @display("bgb=378,324");
    gates:
        inout toNetwork[nicCount];

    submodules:
        application[appCount]: Application {
            @display("p=69,31");
        }
        hostRouter: HostRouter {
            @display("p=122,142");
        }
        nic[nicCount]: NIC {
            @display("p=122,283");
        }
        ramManager: RAMManager {
            @display("p=326,41");
        }
        ioManager: IOManager {
            @display("p=327,169");
        }
        cpuManager: CPUManager {
            @display("p=327,97");
        }
    connections allowunconnected:
        for i=0..appCount-1 {
            application[i].downLink <--> hostRouter.upLink++;
        }
        for i=0..nicCount-1 {
            nic[i].upLink <--> hostRouter.downLink++;
            nic[i].downLink <--> toNetwork++;
        }

}
