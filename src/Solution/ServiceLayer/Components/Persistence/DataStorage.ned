// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.ServiceLayer.Components.Persistence;

//
// TODO auto-generated module
//
simple DataStorage
{
    parameters:        
    	@display("i=block/buffer2,#008000");
    	string performanceDescriptor;
    	bool isStandAlone = default(false);
    gates:
        inout dbLink;
}
