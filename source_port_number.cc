#include "ns3/packet.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "source_port_number.h"
#include "ns3/ppp-header.h"
#include "ns3/ipv4-header.h"

#include <string>

/**
 * SourcePortNumber class is the Filter element that matches the source port number
 */

using namespace ns3;

SourcePortNumber::SourcePortNumber(std::string port)
    : m_port(atoi(port.c_str()))
{}

/**
 * Match the source port number of the packet with the source port number of the filter element.
 * @param packet
 * @return true if the source port number of the packet matches the source port number of the filter element.
 */
bool SourcePortNumber::match(Ptr<Packet> packet)
{

    bool matchParam = false;


    PppHeader ppp;
    packet->RemoveHeader(ppp);
    Ipv4Header ip;
    packet->RemoveHeader(ip);
    uint32_t protocol = ip.GetProtocol();

    if(protocol == 17){
        UdpHeader udp;
        packet->PeekHeader(udp);
        matchParam = (m_port == udp.GetSourcePort());
    }else if(protocol == 6){
        TcpHeader tcp;
        packet->PeekHeader(tcp);
        matchParam = (m_port == tcp.GetSourcePort());
    }

    packet->AddHeader(ip);
    packet->AddHeader(ppp);

    return matchParam;
}

std::string SourcePortNumber::type()
{
    return "source_port_number";
}
