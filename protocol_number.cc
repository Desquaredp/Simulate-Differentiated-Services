#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "protocol_number.h"
#include <string>

/**
 * ProtocolNumber class is the Filter element that matches the protocol number
 */
using namespace ns3;

ProtocolNumber::ProtocolNumber(std::string protocol)
    : m_protocol(atoi(protocol.c_str()))
{}

/**
 * Match the protocol number of the packet with the protocol number of the filter
 * @param packet
 * @return
 */
bool ProtocolNumber::match(Ptr<Packet> packet)
{

    Ipv4Header header;
    packet->RemoveHeader(header);

    if (header.GetProtocol() == m_protocol) {
        packet->AddHeader(header);
        return true;
    }

    packet->AddHeader(header);

    return false;
}

std::string ProtocolNumber::type()
{
    return "protocol_number";
}
