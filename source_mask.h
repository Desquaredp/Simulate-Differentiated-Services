#ifndef SOURCE_MASK_H
#define SOURCE_MASK_H


#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "filter_element.h"
#include <string>

namespace ns3 {
class SourceMask:public FilterElement
{
  public:
    SourceMask(std::string sourceIpMask);
    bool match(Ptr<Packet> packet);
    std::string type();

private:
    Ipv4Mask m_mask;
    Ipv4Address m_address;
};

} // namespace ns3

#endif // SOURCE_MASK_H
