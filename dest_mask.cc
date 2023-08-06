#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "dest_mask.h"
#include <string>

/**
 * Filter Element that matches the masked destination IP address of a packet.
 *
 */

using namespace ns3;

DestinationMask::DestinationMask(std::string destIpMask)
{
    std::string mask = destIpMask.substr(destIpMask.find('/') + 1);
    std::string address = destIpMask.substr(0, destIpMask.find('/'));
    m_mask = Ipv4Mask(mask.c_str());
    m_address = Ipv4Address(address.c_str());
}

/**
 * Returns true if the masked destination IP address of the packet matches the address.
 * @param packet
 * @return
 */
bool DestinationMask::match(Ptr<Packet> packet)
{
    //print sth that shows that this match function is called
    Ipv4Header header;
    packet->PeekHeader(header);
    // print the masked address
    if (header.GetDestination().CombineMask(m_mask) == m_address) {
        return true;
    }
    return 0;
}

std::string DestinationMask::type()
{
    return "DestinationMask";
}
