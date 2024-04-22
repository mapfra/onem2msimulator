// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com
 

package oneM2M.Solution.ServiceLayer.Components;

//
// TODO auto-generated module
//
simple CSERouter
{
    @display("i=block/broadcast");
    gates:
        inout httpLink;
        inout coapLink;
        inout mqttLink;
        inout dbLink;
        
        inout downLink;
}
