// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.InfrastructureLayer.Nodes;

import oneM2M.Solution.InfrastructureLayer.Nodes.Components.NIC;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.HostRouter;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.CPUManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.IOManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.RAMManager;
import oneM2M.Solution.ServiceLayer.CSE;
import oneM2M.Solution.ServiceLayer.Components.Communication.MQTTBrokerService;


module CSENode extends HWNode
{
    parameters:
        @display("i=device/device;bgb=,,#F8F8F8");
        bool hasLocalMQTTBroker = default(false);
		bool hasLocalDB = default(true);

    submodules:
        cse: CSE {
            @display("p=161,31;is=n");
            name = parent.name;
            hasLocalDB = parent.hasLocalDB;
        }
        mqttBroker: MQTTBrokerService if hasLocalMQTTBroker {
            @display("p=233,30");
        }
    connections allowunconnected:
        hostRouter.upLink++ <--> mqttBroker.downLink if hasLocalMQTTBroker;
        hostRouter.upLink++ <--> cse.downLink;
}

