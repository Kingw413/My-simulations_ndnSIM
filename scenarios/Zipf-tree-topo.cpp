// Zipf-tree-topo.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 1);
  topologyReader.SetFileName("/home/whd/whd_ndnSIM2.1/my-simulations/topologies/topo-tree.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "100");
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/root", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> consumers[4] = {Names::Find<Node>("leaf-1"), Names::Find<Node>("leaf-2"),
                            Names::Find<Node>("leaf-3"), Names::Find<Node>("leaf-4")};
  Ptr<Node> producer = Names::Find<Node>("root");

  for (int i = 0; i < 4; i++) {
    ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
    consumerHelper.SetAttribute("Frequency", StringValue("100")); // 10 interests a second
    consumerHelper.SetAttribute("NumberOfContents", StringValue("100"));
    consumerHelper.SetAttribute("q", StringValue("0"));
    consumerHelper.SetAttribute("s", StringValue("0.7"));

    // Each consumer will express unique interests /root/<leaf-name>/<seq-no>
    consumerHelper.SetPrefix("/root/" + Names::FindName(consumers[i]));
    consumerHelper.Install(consumers[i]);
  }

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  // Register /root prefix with global routing controller and
  // install producer that will satisfy Interests in /root namespace
  ndnGlobalRoutingHelper.AddOrigins("/root", producer);
  producerHelper.SetPrefix("/root");
  producerHelper.Install(producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(20.0));

  ndn::CsTracer::InstallAll("Zipf-tree-topo_cs-trace.txt", Seconds(0.5));
  ndn::L3RateTracer::InstallAll("Zipf-tree-topo_rate-trace.txt", Seconds(0.5));
  ndn::AppDelayTracer::InstallAll("Zipf-tree-topo_app-delays-trace.txt");

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