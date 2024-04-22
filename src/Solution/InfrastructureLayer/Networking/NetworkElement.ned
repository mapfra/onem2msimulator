// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.InfrastructureLayer.Networking;

//
// TODO auto-generated module
//
simple NetworkElement
{
    parameters:
    	@display("i=abstract/router");
    	string subnetAddress; //ADD 
    gates:
        inout link[];
        inout internetLink;
}
