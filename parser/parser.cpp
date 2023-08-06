#include <iostream>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;

int main() {
    std::ifstream file("config.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    xml_node<>* queue = doc.first_node("queue");
    if (queue) {
        for (xml_node<>* item = queue->first_node("item"); item; item = item->next_sibling("item")) {
            std::string name = item->first_node("name")->value();
            int weight = std::stoi(item->first_node("weight")->value());
            std::cout << "Name: " << name << ", Weight: " << weight << std::endl;

            for (xml_node<>* filterElement = item->first_node("filters")->first_node("filterElements"); filterElement; filterElement = filterElement->next_sibling("filterElements")) {
                std::string ip = filterElement->first_node("ip")->value();
                int port = std::stoi(filterElement->first_node("port")->value());
                std::cout << "  IP: " << ip << ", Port: " << port << std::endl;
            }
        }
    }

    return 0;
}

