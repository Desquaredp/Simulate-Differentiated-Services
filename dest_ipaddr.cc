#include "ns3/core-module.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-header.h"
#include "dest_ipaddr.h"
#include <string>

/**
 * Filter Element that matches the destination IP address of a packet.
 *
 */

using namespace ns3;

DestinationIPAddress::DestinationIPAddress(std::string address)
{
    m_address = Ipv4Address(address.c_str());
}

/**
 * Returns true if the destination IP address of the packet matches the address
 * @param packet
 * @return
 */
bool DestinationIPAddress::match(Ptr<Packet> packet)
{

    Ipv4Header header;
    packet->PeekHeader(header);
    if (header.GetDestination() == m_address) {
        return true;
    }
    return false;


}

std::string DestinationIPAddress::type()
{
    return "DestinationIPAddress";
}

