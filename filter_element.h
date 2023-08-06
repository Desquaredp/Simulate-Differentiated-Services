#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/packet.h"
#include <vector>
#include <memory>

namespace ns3 {

    class FilterElement
    {
        public:
            virtual ~FilterElement() {}
            virtual bool match(Ptr<Packet> packet) = 0;
            virtual std::string type() = 0;
    };


} // namespace ns3

#endif // FILTER_ELEMENT_H
