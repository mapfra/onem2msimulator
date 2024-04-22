// Author : Samir MEDJIAH
// Email : samir.medjiah@gmail.com


package oneM2M.Solution.InfrastructureLayer.Networking;

//
// TODO auto-generated type
//
channel WifiLink extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 100us;
    ber = 0;
    per = 1e-5;
    @display("ls=#008000,2,da");
}

channel CellularLink extends ned.DatarateChannel
{
    datarate = 50Mbps;
    delay = 150us;
    ber = 0;
    per = 1e-5;
    @display("ls=blue,2,d");
}

channel LPWANLink extends ned.DatarateChannel
{
    datarate = 1000 bps;
    delay = 500ms;
    ber = 0;
    per = 1e-5;
    @display("ls=navy,2,d");
}

channel FiberLink extends ned.DatarateChannel
{
    datarate = 500Mbps;
    delay = 10us;
    ber = 0;
    per = 1e-5;
    @display("ls=black,2,s");
}
