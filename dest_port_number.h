#ifndef DESTINATION_PORT_NUMBER_H
#define DESTINATION_PORT_NUMBER_H

#include "ns3/packet.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include <string>
#include "filter_element.h"

namespace ns3 {

    class DestinationPortNumber : public FilterElement
    {
        public:
            DestinationPortNumber(std::string port);
            bool match(Ptr<Packet> packet);
            std::string type();

        private:
            uint32_t m_port;
    };

} // namespace ns3

#endif // DESTINATION_PORT_NUMBER_H
