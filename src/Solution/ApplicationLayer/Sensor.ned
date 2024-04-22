// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.ApplicationLayer;

//
// TODO auto-generated module
//
simple Sensor
{
    parameters:
        string name;
        string cinGenerator = default("");
        
        @display("i=block/tx");
    gates:
        inout downLink;
}
