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



network SimpleNetwork
{
    parameters:
		// name: Traffic Light Use Case
		// description: A simple traffic light scenario with 3 CSEs, 5 IoTNodes connected to 3 different access networks 
		// authors: TTF T019 Experts
		// version: 1.0
		
    submodules:
        cloudServer: ServerNode {
            @display("p=269,535;is=l");
                    
        }
        mnNode1: CSENode {
            @display("p=269,92");
        }
        mnNode2: CSENode {
            @display("p=240,270");
        }
        inNode: CSENode {
            @display("p=269,457");
        }
        FiberNet: NetworkElement {
            @display("p=369,472");
        }
        INTERNET: Internet {
            @display("p=529,273");
        }
        Cellular: NetworkElement {
            @display("p=358,170");
        }
        WiFi: NetworkElement {
            @display("p=358,316");
        }
        intersection01: IoTNode {
            @display("p=123,91");
        }
        intersection02: IoTNode {
            @display("p=123,159");
        }
        intersection03: IoTNode {
            @display("p=117,221");
        }
        intersection04: IoTNode {
            @display("p=123,322");
        }
        intersection05: IoTNode {
            @display("p=130,390");
        }
    connections:
        mnNode1.toNetwork[0] <--> CellularLink <--> Cellular.link++;
        intersection03.toNetwork[0] <--> CellularLink <--> Cellular.link++;
        intersection02.toNetwork[0] <--> CellularLink <--> Cellular.link++;
        intersection01.toNetwork[0] <--> CellularLink <--> Cellular.link++;
                
        mnNode2.toNetwork[0] <--> WifiLink <--> WiFi.link++;
        intersection04.toNetwork[0] <--> WifiLink <--> WiFi.link++;
        intersection05.toNetwork[0] <--> WifiLink <--> WiFi.link++;

        inNode.toNetwork[0] <--> FiberLink <--> FiberNet.link++;
        cloudServer.toNetwork[0] <--> FiberLink <--> FiberNet.link++;
                
        WiFi.internetLink <--> FiberLink <--> INTERNET.link++;
        Cellular.internetLink <--> FiberLink <--> INTERNET.link++;
        FiberNet.internetLink <--> FiberLink <--> INTERNET.link++;

}
