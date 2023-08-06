#include "filter_elements_factory.h"
#include "filter_element.h"
#include "dest_ipaddr.h"
#include "source_ipaddr.h"
#include "dest_port_number.h"
#include "source_port_number.h"
#include "protocol_number.h"
#include "source_mask.h"
#include "dest_mask.h"
#include <string>
#include <iostream>

/**
 * FilterElementFactory class is to create a filter element object depending on the type of the filter elements declared in the config file.
 */
using namespace ns3;

FilterElementFactory::FilterElementFactory()
{
}

FilterElementFactory::~FilterElementFactory()
{
}

/**
 * This method creates a filter element object depending on the type of the filter elements declared in the config file.
 * @param type - type of the filter element
 * @param value - value of the filter element
 * @return - a filter element object
 */
FilterElement* FilterElementFactory::createFilterElement(std::string type, std::string value)
{
    if (type == "destination_ip_address") {

        return new DestinationIPAddress(value);
    }else if (type == "source_ip_address") {

        return new SourceIPAddress(value);
    }else if (type == "destination_port") {

        return new DestinationPortNumber(value);

    }else if (type == "source_port") {

        return new SourcePortNumber(value);
    }else if (type == "protocol") {

        return new ProtocolNumber(value);
    }else if (type == "destination_mask")
    {
        return new DestinationMask(value);
    }else if (type == "source_mask")
    {
        return new SourceMask(value);
    }
    else {
        std::cout << "Invalid filter element type" << std::endl;
    }


    return nullptr;
}