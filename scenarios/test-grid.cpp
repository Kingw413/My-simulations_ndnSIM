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
        Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
        Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("10"));

        CommandLine cmd;
        cmd.Parse(argc, argv);

        //Creating Topologies
        PointToPointHelper p2p;
        PointToPointGridHelper grid(3, 3, p2p);
        grid.BoundingBox(100, 100, 200, 200);

        //Install NDN stack on all nodes;
        ndn::StackHelper ndnHelper;
        ndnHelper.InstallAll();

        //Choosing forwarding strategy
        ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");
        
        //Install Routing
        ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
        ndnGlobalRoutingHelper.InstallAll();

        Ptr<Node> producer = grid.GetNode(2, 2);
        NodeContainer consumerNodes;
        consumerNodes.Add(grid.GetNode(0,0));

        std::string prefix="/prefix";
        ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
        consumerHelper.SetPrefix(prefix);
        consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second
        consumerHelper.Install(consumerNodes);

        ndn::AppHelper producerHelper("ns3::ndn::Producer");
        producerHelper.SetPrefix(prefix);
        producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
        producerHelper.Install(producer);

        // Add /prefix origins to ndn::GlobalRouter
        ndnGlobalRoutingHelper.AddOrigins(prefix, producer);

        // Calculate and install FIBs
        ndn::GlobalRoutingHelper::CalculateRoutes();

        Simulator::Stop(Seconds(20.0));

        Simulator::Run();
        Simulator::Destroy();

        return 0;
    }

} // namespace ns3

int main(int argc, char *argv[])
{
    return ns3::main(argc, argv);
}