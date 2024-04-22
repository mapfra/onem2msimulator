//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package oneM2M.Solution.InfrastructureLayer.Nodes;

import oneM2M.Solution.InfrastructureLayer.Nodes.HWNode;

import oneM2M.Solution.InfrastructureLayer.Nodes.Components.NIC;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.HostRouter;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.CPUManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.IOManager;
import oneM2M.Solution.InfrastructureLayer.Nodes.Components.RAMManager;

import oneM2M.Solution.ServiceLayer.Components.Persistence.DataStorage;
import oneM2M.Solution.ServiceLayer.Components.Communication.MQTTBrokerService;

import ned.IUnidirectionalChannel;



module ServerNode extends HWNode
{
    parameters:
        @display("i=device/server");
        bool hasMQTTBroker = default(false);
        bool hasDataBase = default(false);

    submodules:
        mqttBroker: MQTTBrokerService if hasMQTTBroker {
            @display("p=233,30");
        }
        database: DataStorage if hasDataBase {
            @display("p=161,31");
        }
    connections allowunconnected:
        hostRouter.upLink++ <--> mqttBroker.downLink if hasMQTTBroker;
        hostRouter.upLink++ <--> database.dbLink if hasDataBase;
}
