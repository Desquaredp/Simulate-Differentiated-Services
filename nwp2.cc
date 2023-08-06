
#include "../../src/internet/model/ipv4-header.h"
#include "filter.h"
#include "filter_element.h"
#include "filter_elements_factory.h"
#include "parser/rapidxml.hpp"

#include "spq.h"
#include "traffic_class.h"
#include "drr.h"

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/gnuplot.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace rapidxml;
using namespace ns3;

void SpqSiml(Ptr<Spq>);
void DrrSiml(Ptr<Drr>);
void populate(xml_node<>* pNode, FilterElementFactory factory, std::vector<FilterElement*>* vector1);

/**
 * Comparator for the priority queue
 * Used for sorting the TrafficClass vector-so the queues based on their priority or weights
 */
struct Comp{
    bool operator()( TrafficClass& t1,  TrafficClass& t2){
        if(t1.getPriorityLevel() != 0 && t2.getPriorityLevel() != 0){
            return t1.getPriorityLevel() > t2.getPriorityLevel();
        }
        else{
            return t1.getWeight() > t2.getWeight();
        }
    }
};

int main(int argc, char* argv[]){

    ns3::PacketMetadata::Enable ();

    std::string strArg = "configFileName";

    CommandLine cmd;
    cmd.AddValue("strArg", "Name of the configuration file", strArg);
    cmd.Parse(argc, argv);

    std::string configFileName = strArg;

    std::cout << "Config file name: " << configFileName << std::endl;

    // concatanate config file name with the path scratch/nwp2/
    configFileName = "scratch/nwp2/" + configFileName;

    std::ifstream file(configFileName);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* queue = doc.first_node("queues");
    FilterElementFactory newFactory = FilterElementFactory();
    std::vector<TrafficClass*> trafficClasses;
    bool isPriority = false;
    bool isWeight = false;

    if (queue) {
        for (xml_node<>* item = queue->first_node("queue"); item; item = item->next_sibling("queue")) {
            TrafficClass* newTrafficClass = new TrafficClass();
            if (item->first_node("name")) {
                std::string name = item->first_node("name")->value();
                newTrafficClass->setName(name);
            }

            //check if the traffic class is priority or weighted fair
            if (item->first_node("priority")) {
                if (isWeight) {
                    std::cout << "Error: Both priority and weight specified" << std::endl;
                    return 0;
                }


                int priority = std::stoi(item->first_node("priority")->value());
                newTrafficClass->setPriorityLevel(priority);
                isPriority = true;
            }else if (item->first_node("weight")) {
                if (isPriority) {
                    std::cout << "Error: Both priority and weight specified" << std::endl;
                    return 0;
                }

                double weight = std::stod(item->first_node("weight")->value());
                newTrafficClass->setWeight(weight);
                isWeight = true;
            }else{
                std::cout << "Error: No priority or weight specified" << std::endl;
                return 0;
            }

            if (item->first_node("is_default")) {

                std::string isDefault = item->first_node("is_default")->value();
                if (isDefault == "true") {
                    newTrafficClass->setIsDefault(true);
                }
            }


            for (xml_node<>* filterElement = item->first_node("filters")->first_node("filter_elements"); filterElement; filterElement = filterElement->next_sibling("filter_elements")) {

                Filter* newFilter = new Filter();
                std::vector<FilterElement*> filterElements;

                populate(filterElement, newFactory, &filterElements);

                newFilter->addAllFilterElements(filterElements);
                newTrafficClass->addFilter(newFilter);


            }

            trafficClasses.push_back(newTrafficClass);

        }
    }

    std::priority_queue<TrafficClass, std::vector<TrafficClass>, Comp> pq;
    for(auto trafficClass : trafficClasses){
        pq.push(*trafficClass);
    }

    std::vector<TrafficClass*> trafficClasses2;

    while(!pq.empty()){
        TrafficClass* trafficClass = new TrafficClass();
        *trafficClass = pq.top();
        trafficClasses2.push_back(trafficClass);
        pq.pop();
    }



    if (isWeight)
    {
        std::cout << "Using DRR as QoS" << std::endl;

        //Create a pointer to the Drr object
        Ptr<Drr> drr = CreateObject<Drr>();
        drr->SetTrafficClasses(trafficClasses);
        drr->InitializeDeficitCounter();
        drr->SetDefaultQuantum(500);
        DrrSiml(drr);

    }else if (isPriority){
        std::cout << "Using SPQ as QoS" << std::endl;

        //Create a pointer to the Spq object
        Ptr<Spq> spq = CreateObject<Spq>();
        spq->SetTrafficClasses(trafficClasses2);
        SpqSiml(spq);
    }else{
        std::cout << "Error: No priority or weight specified" << std::endl;
        return 0;
    }


    return 0;
}

/**
 * Simulation scenario for DRR QoS. Three node topology with two point-to-point links.
 * The first link has a data rate of 4Mbps and the second link has a data rate of 1Mbps.
 * The first node sends a packet to the third node.
 * @param drr
 */
void DrrSiml( Ptr<Drr> drr )
{

    // Create three nodes
    NodeContainer nodes;
    nodes.Create (3);

    // Create point-to-point links between the nodes
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer devices1, devices2;
    devices1 = p2p.Install (nodes.Get (0), nodes.Get (1));
    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
    devices2 = p2p.Install (nodes.Get (1), nodes.Get (2));

    Ptr<PointToPointNetDevice> m_device = DynamicCast<PointToPointNetDevice>(devices2.Get(0));
    m_device->SetQueue(drr);

    // Assign IP addresses to the nodes
    InternetStackHelper stack;
    stack.Install (nodes);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);

    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);

    // Enable routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


    // Install applications on nodes
    UdpServerHelper server (3);
    ApplicationContainer apps = server.Install (nodes.Get (2));
    apps.Start (Seconds (0.0));
    apps.Stop (Seconds (15.0));

    // Install applications on nodes
    UdpServerHelper serverApp (2);
    ApplicationContainer apps2 = serverApp.Install (nodes.Get (2));
    apps2.Start (Seconds (0.0));
    apps2.Stop (Seconds (15.0));

    // Install applications on nodes
    UdpServerHelper serverApp3 (1);
    ApplicationContainer apps3 = serverApp3.Install (nodes.Get (2));
    apps3.Start (Seconds (0.0));
    apps3.Stop (Seconds (15.0));


    UdpClientHelper client1 (interfaces2.GetAddress (1), 3);
    client1.SetAttribute ("MaxPackets", UintegerValue (10000));
    client1.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
    client1.SetAttribute ("PacketSize", UintegerValue (1024));
    apps = client1.Install (nodes.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (15.0));


    UdpClientHelper client2 (interfaces2.GetAddress (1), 2);
    client2.SetAttribute ("MaxPackets", UintegerValue (10000));
    client2.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
    client2.SetAttribute ("PacketSize", UintegerValue (1024));
    apps2 = client2.Install (nodes.Get (0));
    apps2.Start (Seconds (1.0));
    apps2.Stop (Seconds (15.0));

    UdpClientHelper client3 (interfaces2.GetAddress (1), 1);
    client3.SetAttribute ("MaxPackets", UintegerValue (10000));
    client3.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
    client3.SetAttribute ("PacketSize", UintegerValue (1024));
    apps3 = client3.Install (nodes.Get (0));
    apps3.Start (Seconds (1.0));
    apps3.Stop (Seconds (15.0));


    // Run simulation
    Simulator::Stop (Seconds(15.0));

    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll(ascii.CreateFileStream("drr.tr"));
    p2p.EnablePcapAll("drr_test");


    Simulator::Run ();
    Simulator::Destroy ();

}

/**
 * Simulation scenario for SPQ QoS. Three node topology with two point-to-point links.
 * The first link has a data rate of 40Mbps and the second link has a data rate of 10Mbps.
 * The first node sends a packet to the third node.
 * @param spq
 */
void SpqSiml( Ptr<Spq> spq )
{

     // Create three nodes
     NodeContainer nodes;
     nodes.Create (3);

     // Create point-to-point links between the nodes
     PointToPointHelper p2p;
     p2p.SetDeviceAttribute ("DataRate", StringValue ("40Mbps"));
     p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
     NetDeviceContainer devices1, devices2;
     devices1 = p2p.Install (nodes.Get (0), nodes.Get (1));
     p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
     p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
     devices2 = p2p.Install (nodes.Get (1), nodes.Get (2));


     Ptr<PointToPointNetDevice> m_device = DynamicCast<PointToPointNetDevice>(devices2.Get(0));
     m_device->SetQueue(spq);

     // Assign IP addresses to the nodes
     InternetStackHelper stack;
     stack.Install (nodes);

     Ipv4AddressHelper address;
     address.SetBase ("10.1.1.0", "255.255.255.0");
     Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);

     address.SetBase ("10.1.2.0", "255.255.255.0");
     Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);

     // Enable routing
     Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


     /*
      * Create two UDP client-server
      */

     // Install applications on nodes
     UdpServerHelper server (1);
     ApplicationContainer apps = server.Install (nodes.Get (2));
     apps.Start (Seconds (0.0));
     apps.Stop (Seconds (10.0));

     // Install applications on nodes
     UdpServerHelper serverApp (2);
     ApplicationContainer apps2 = serverApp.Install (nodes.Get (2));
     apps2.Start (Seconds (0.0));
     apps2.Stop (Seconds (10.0));

     UdpClientHelper client1 (interfaces2.GetAddress (1), 2);
     client1.SetAttribute ("MaxPackets", UintegerValue (80000));
     client1.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
     client1.SetAttribute ("PacketSize", UintegerValue (1024));
     apps = client1.Install (nodes.Get (0));
     apps.Start (Seconds (0.0));
     apps.Stop (Seconds (10.0));


     UdpClientHelper client2 (interfaces2.GetAddress (1), 1);
     client2.SetAttribute ("MaxPackets", UintegerValue (50000));
     client2.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
     client2.SetAttribute ("PacketSize", UintegerValue (1024));
     apps2 = client2.Install (nodes.Get (0));
     apps2.Start (Seconds (1.0));
     apps2.Stop (Seconds (10.0));


     // Run simulation
     Simulator::Stop (Seconds(10.0));

     p2p.EnablePcapAll("spq_test");

     Simulator::Run ();

     Simulator::Destroy ();

}


/**
 * This function is used to populate the filter elements
 * @param filterElement is the xml node that contains the filter elements
 * @param factory is the factory that creates the filter elements
 * @param vector1 is the vector that contains the filter elements
 */
void populate(xml_node<>* filterElement, FilterElementFactory factory, std::vector<FilterElement*>* vector1)
{


     if(filterElement->first_node("destination_ip_address") && filterElement->first_node("destination_mask"))
     {
        std::string dest_ip = filterElement->first_node("destination_ip_address")->value();
        std::string dest_mask = filterElement->first_node("destination_mask")->value();
        std::string dest_ip_mask = dest_ip + "/" + dest_mask;
        vector1->push_back(
            factory.createFilterElement("destination_mask",
                                        dest_ip_mask));

     }

     if(filterElement->first_node("source_ip_address") && filterElement->first_node("source_mask"))
     {
        std::string src_ip = filterElement->first_node("source_ip_address")->value();
        std::string src_mask = filterElement->first_node("source_mask")->value();
        std::string src_ip_mask = src_ip + "/" + src_mask;
        vector1->push_back(
            factory.createFilterElement("source_mask",
                                        src_ip_mask));

     }

        if (filterElement->first_node("destination_ip_address"))
        {

            vector1->push_back(factory.createFilterElement(
                "destination_ip_address",
                filterElement->first_node("destination_ip_address")->value()));

        }
        if (filterElement->first_node("source_ip_address"))
        {
            vector1->push_back(factory.createFilterElement(
                "source_ip_address",
                filterElement->first_node("source_ip_address")->value()));
        }

        if (filterElement->first_node("destination_port"))
        {
            vector1->push_back(factory.createFilterElement(
                "destination_port",
                filterElement->first_node("destination_port")->value()));
        }

        if (filterElement->first_node("source_port"))
        {
            vector1->push_back(
                factory.createFilterElement("source_port",
                                            filterElement->first_node("source_port")->value()));
        }

        if (filterElement->first_node("protocol"))
        {
            vector1->push_back(
                factory.createFilterElement("protocol",
                                            filterElement->first_node("protocol")->value()));
        }



}