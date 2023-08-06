#ifndef FILTER_ELEMENT_FACTORY_H
#define FILTER_ELEMENT_FACTORY_H

#include "ns3/packet.h"
#include <vector>
#include <memory>
#include "filter_element.h"

namespace ns3 {

    class FilterElementFactory {

        public:
            FilterElementFactory();
            ~FilterElementFactory();
            FilterElement* createFilterElement(std::string type, std::string value);
    };

}
#endif
