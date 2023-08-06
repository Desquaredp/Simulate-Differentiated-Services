#include "ns3/packet.h"
#include "ns3/tcp-header.h"
#include "dest_port_number.h"
#include "ns3/udp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/uinteger.h"
#include "ns3/ppp-header.h"

#include <string>

/**
 * Filter Element that matches the destination port number of a packet.
 *
 */
using namespace ns3;


DestinationPortNumber::DestinationPortNumber(std::string port)
    : m_port(atoi(port.c_str()))
{}

/**
 * Returns true if the destination port number of the packet matches the port number.
 * @param packet
 * @return
 */
bool DestinationPortNumber::match(Ptr<Packet> packet)
{

    bool matchParam = false;


    PppHeader ppp;
    packet->RemoveHeader(ppp);
    Ipv4Header ip;
    packet->RemoveHeader(ip);
    uint32_t protocol = ip.GetProtocol();

    if (protocol == 17){
        UdpHeader udp;
        packet->PeekHeader(udp);
        matchParam = (m_port == udp.GetDestinationPort());

    }else if (protocol == 6){
        TcpHeader tcp;
        packet->PeekHeader(tcp);
        matchParam = (m_port == tcp.GetDestinationPort());
    }

    packet->AddHeader(ip);
    packet->AddHeader(ppp);
    return matchParam;

}

std::string DestinationPortNumber::type()
{
    return "destination_port_number";
}
