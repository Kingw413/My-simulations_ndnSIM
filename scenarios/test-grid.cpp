// test-grid.cpp

#include "ns3/core-module.h"
#include  "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3{

    int main(int argc, char* argv[])
    {
        /* code */
        //Setting default parameters for P2P links and channels
        Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
        Config::SetDefault("ns3::PointToPointChannel::Delay", StringValur("10ms"));
        Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("10"));

        CommandLine cmd;
        cmd.Parse(argc, argv);

        ndn::Stack
        return 0;
    }
    
}