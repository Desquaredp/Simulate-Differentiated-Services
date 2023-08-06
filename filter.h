
#ifndef FILTER_H
#define FILTER_H

#include "ns3/packet.h"
#include <vector>
#include <memory>
#include "filter_element.h"

namespace ns3 {

    class Filter
    {
        public:
            Filter();
            ~Filter();
            void addFilterElement(FilterElement* filterElement);
            void addAllFilterElements(std::vector<FilterElement*> filterElements);
            bool match(Ptr<Packet> packet);
            std::vector<FilterElement*> getFilterElements();

        private:
            std::vector<FilterElement*> m_filterElements;
    };

} // namespace ns3

#endif // FILTER_H
