// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com

package oneM2M.Solution.InfrastructureLayer.Nodes.Components;

//
// TODO auto-generated module
//
simple NIC
{
    
    parameters:
    	@display("i=block/ifcard");
    	string name = default("nic0");
    	string networkAddress;
    	
    gates:
        inout upLink;		// links to bindings
        inout downLink;		// link to network
}
