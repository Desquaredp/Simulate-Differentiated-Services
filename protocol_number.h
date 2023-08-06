#ifndef PROTOCOL_NUMBER_H
#define PROTOCOL_NUMBER_H

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv6-header.h"
#include "filter_element.h"
#include <string>

namespace ns3 {

    class ProtocolNumber : public FilterElement
    {
        public:
            ProtocolNumber(std::string protocol);
            bool match(Ptr<Packet> packet);
            std::string type();

        private:
            uint32_t m_protocol;
    };

} // namespace ns3

#endif // PROTOCOL_NUMBER_H
