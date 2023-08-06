#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "source_mask.h"
#include <string>

/**
 *Filter Element that matches the masked source IP address of a packet.
 */
using namespace ns3;

SourceMask:: SourceMask(std::string sourceIpMask)
{
    std::string mask = sourceIpMask.substr(sourceIpMask.find('/') + 1);
    std::string address = sourceIpMask.substr(0, sourceIpMask.find('/'));
    m_mask = Ipv4Mask(mask.c_str());
    m_address = Ipv4Address(address.c_str());

}

/**
 * Returns true if the masked source IP address of the packet matches the address.
 * @param packet
 * @return
 */
bool SourceMask::match(Ptr<Packet> packet)
{
    Ipv4Header header;
    packet->PeekHeader(header);
    if (header.GetSource().CombineMask(m_mask) == m_address) {
        return true;
    }
    return 0;
}

std::string SourceMask::type()
{
    return "SourceMask";
}
