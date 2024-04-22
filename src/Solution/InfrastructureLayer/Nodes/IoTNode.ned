// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com
 

package oneM2M.Solution.InfrastructureLayer.Nodes;

import oneM2M.Solution.InfrastructureLayer.Nodes.Components.NIC;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.HostRouter;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.CPUManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.IOManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.RAMManager;

import ned.IUnidirectionalChannel;



module IoTNode extends HWNode
{
    parameters:
        @display("i=device/accesspoint");


    connections allowunconnected:

}
