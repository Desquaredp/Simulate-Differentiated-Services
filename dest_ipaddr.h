#ifndef DESTINATION_IP_ADDRESS_H
#define DESTINATION_IP_ADDRESS_H

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "filter_element.h"
#include <string>

namespace ns3 {

    class DestinationIPAddress : public FilterElement
    {
        public:
            DestinationIPAddress(std::string address);
            bool match(Ptr<Packet> packet);
            std::string type();

        private:
            Ipv4Address m_address;
    };

} // namespace ns3

#endif // DESTINATION_IP_ADDRESS_H
