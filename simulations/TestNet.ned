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

// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

package onem2msimulator.simulations;
import oneM2M.Solution.InfrastructureLayer.Networking.CellularLink;
import oneM2M.Solution.InfrastructureLayer.Networking.FiberLink;
import oneM2M.Solution.InfrastructureLayer.Networking.WifiLink;
import oneM2M.Solution.InfrastructureLayer.Networking.NetworkElement;
import oneM2M.Solution.InfrastructureLayer.Networking.Internet;
import oneM2M.Solution.InfrastructureLayer.Nodes.CSENode;
import oneM2M.Solution.InfrastructureLayer.Nodes.IoTNode;
import oneM2M.Solution.InfrastructureLayer.Nodes.ServerNode;



network TestNetwork
{
    parameters:
		// name: Traffic Light Use Case (test)
		// description: A very basic traffic light scenario with 2 CSEs, 1 IoTNode for test purposes 
		// authors: TTF T019 Experts
		// version: 1.0

    submodules:
        mnNode: CSENode {
            @display("p=299,165");
        }
        inNode: CSENode {
            @display("p=291,408");
        }
        FiberNet: NetworkElement {
            @display("p=176,336");
        }
        INTERNET: Internet {
            @display("p=291,270");
        }
        Cellular: NetworkElement {
            @display("p=190,205");
        }
        device: IoTNode {
            @display("p=123,91");
        }
    connections:
        mnNode.toNetwork[0] <--> CellularLink <--> Cellular.link++;
        device.toNetwork[0] <--> CellularLink <--> Cellular.link++;

        inNode.toNetwork[0] <--> FiberLink <--> FiberNet.link++;
        Cellular.internetLink <--> FiberLink <--> INTERNET.link++;
        FiberNet.internetLink <--> FiberLink <--> INTERNET.link++;

}
