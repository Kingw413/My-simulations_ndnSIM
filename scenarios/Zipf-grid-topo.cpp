// Zipf-grid-topo.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3{

int 
main(int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    AnnotatedTopologyReader topologyReader("", 1);
    topologyReader.SetFileName("/home/whd/whd_ndnSIM2.1/my-simulations/topologies/topo-grid-3x3.txt");
    topologyReader.Read();

    //Install NDN stack on all nodes
    ndn::StackHelper ndnHelper;
    ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "100");
    ndnHelper.InstallAll();

    //Choosing forwarding strategy
    ndn::StrategyChoiceHelper::InstallAll("/prefix", "localhost/nfd/strategy/best-route");

    //Installing global routing interface on all nodes
    ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
    ndnGlobalRoutingHelper.InstallAll();

    // Getting containers for the consumer/producer
    Ptr<Node> consumer = Names::Find<Node>("Node0");
    Ptr<Node> producer = Names::Find<Node>("Node8");

    // Install NDN applications
    std::string prefix = "/prefix";

    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper.SetPrefix(prefix);
    consumerHelper.SetAttribute("Frequency", StringValue("100")); // 10 interests a second
    consumerHelper.SetAttribute("NumberOfContents", StringValue("100"));
    consumerHelper.SetAttribute("q", StringValue("0"));
    consumerHelper.SetAttribute("s", StringValue("0.7"));
    consumerHelper.Install(consumer);

    ndn::AppHelper producerHelper("ns3::ndn::Producer");
    producerHelper.SetPrefix(prefix);
    producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper.Install(producer);

    // Add /prefix origins to ndn::GlobalRouter
    ndnGlobalRoutingHelper.AddOrigins(prefix, producer);

    // Calculate and install FIBs
    ndn::GlobalRoutingHelper::CalculateRoutes();

    ndn::CsTracer::InstallAll("Zipf-grid-topo_cs-trace.txt", Seconds(0.5));
    ndn::L3RateTracer::InstallAll("Zipf-grid-topo_rate-trace.txt", Seconds(0.5));
    ndn::AppDelayTracer::InstallAll("Zipf-grid-topo_app-delays-trace.txt");

    Simulator::Stop(Seconds(20.0));

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}