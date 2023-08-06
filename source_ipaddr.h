#ifndef SOURCE_IP_ADDRESS_H
#define SOURCE_IP_ADDRESS_H

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "filter_element.h"
#include <string>

namespace ns3{

class SourceIPAddress : public FilterElement
{
  public:
    SourceIPAddress(std::string address);
    bool match(Ptr<Packet> packet);
    std::string type();

  private:
    Ipv4Address m_address;
};

}
#endif // SOURCE_IP_ADDRESS_H
