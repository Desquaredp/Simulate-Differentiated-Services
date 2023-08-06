#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include <string>
#include "filter.h"
#include "filter_element.h"
#include <vector>
#include "ns3/log.h"

/**
 * Filter class creates containers for the filter elements defined in the config file.
 */
namespace ns3 {

    Filter::Filter() {
    }

    Filter::~Filter() {}

    /**
     * Adds a filter element to the filter.
     * @param filterElement
     */
    void Filter::addFilterElement(FilterElement* filterElement) {
        m_filterElements.push_back(filterElement);
    }

    /**
     * Adds the vector of filter elements to the filter.
     * @param filterElements
     */
    void Filter::addAllFilterElements(std::vector<FilterElement*> filterElements) {
        for (auto filterElement : filterElements) {
            m_filterElements.push_back(filterElement);
        }
    }

    /**
     * Checks if the packet matches the filter.
     * @param packet
     * @return true
     * @return false
     */
    //AND logic
    bool Filter::match(Ptr<Packet> packet) {
        for (auto filterElement : m_filterElements) {

            if (!filterElement->match(packet)) {
                return false;
            }
        }
        return true;
    }

    /**
     * Returns the filter elements of the filter.
     * @return std::vector<FilterElement*>
     */
    std::vector<FilterElement*> Filter::getFilterElements() {
        return m_filterElements;
    }

} // namespace ns3
