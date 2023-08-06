#ifndef SOURCE_PORT_NUMBER_H
#define SOURCE_PORT_NUMBER_H

#include "ns3/packet.h"
#include <string>
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "filter_element.h"

namespace ns3 {

    class SourcePortNumber : public FilterElement
    {
        public:
            SourcePortNumber(std::string port);
            bool match(Ptr<Packet> packet);
            std::string type();

        private:
            uint32_t m_port;
    };

} // namespace ns3

#endif // SOURCE_PORT_NUMBER_H
