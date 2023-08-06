#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include <string>

#include "source_ipaddr.h"

/**
 * SourceIpAddress class is the Filter class that matches the source IP address of the packet.
 */
using namespace ns3;

SourceIPAddress::SourceIPAddress(std::string address){
    m_address = Ipv4Address(address.c_str());
}

/**
 * match function returns true if the source IP address of the packet matches the address.
 * @param packet
 * @return true if the source IP address of the packet matches the address.
 */
bool SourceIPAddress:: match(Ptr<Packet> packet)
{

       Ipv4Header header;
       packet->PeekHeader(header);
       if (header.GetSource() == m_address) {
           return true;
       }
       return false;


}

std::string SourceIPAddress::type()
{
    return "source_ip_address";
}
