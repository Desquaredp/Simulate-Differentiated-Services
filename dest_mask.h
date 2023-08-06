#ifndef DESTINATION_MASK_H
#define DESTINATION_MASK_H

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "filter_element.h"
#include <string>

namespace ns3 {

class DestinationMask: public FilterElement
{
  public:
    DestinationMask(std::string destIpMask);
    bool match(Ptr<Packet> packet);
    std::string type();

  private:
    Ipv4Mask m_mask;
    Ipv4Address m_address;
};

} // namespace ns3

#endif // DESTINATION_MASK_H