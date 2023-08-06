#include "ns3/core-module.h"
#include "traffic_class.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "filter.h"

/**
 * TrafficClass is a class that represents a traffic class in a DiffServ. It has the properties of the queues defined in the configuration file.
 * It contains the queue, the vector of Filters and Filters containing the Filter elements that are defined in the configuration file.
 */

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("TrafficClass");

TrafficClass::TrafficClass() {
  packets = 0;
  max_packets = 100;
  weight = 0;
  priority_level = 0;
  isDefault = false;

}

void TrafficClass::setName(std::string QueueName) {
  this->name = QueueName;
}

std::string TrafficClass::getName() {
  return name;
}

void TrafficClass::setPackets(uint32_t num) {
  packets = num;
}

uint32_t TrafficClass::getPackets() {
  return packets;
}

void TrafficClass::setMaxPackets(uint32_t num) {
  max_packets = num;
}

uint32_t TrafficClass::getMaxPackets() {
  return max_packets;
}

void TrafficClass::setWeight(double num) {
  weight = num;
}

double TrafficClass::getWeight() {
  return weight;
}

void TrafficClass::setPriorityLevel(uint32_t num) {
  priority_level = num;
}

uint32_t TrafficClass::getPriorityLevel() {
  return priority_level;
}

void TrafficClass::setIsDefault(bool check) {
  isDefault = check;
}


void TrafficClass::setFilters(std::vector<Filter*> filtersVector) {
  this->filters = filtersVector;
}

/**
 * Add a filter to the vector of filters.
 * @param filter
 */
void TrafficClass::addFilter(Filter* filter) {
  filters.push_back(filter);
}

std::vector<Filter*> TrafficClass::getFilters() {
  return filters;
}

/**
 * Match the packet to the corresponding filter. Among all the filters there is an OR relation.
 * If any of the Filter matches the packet, return true.
 * @param p
 * @return
 */
bool TrafficClass::match(Ptr<Packet> p) {

  NS_LOG_FUNCTION(this);

  //OR
  for (auto filter : filters) {
    if (filter->match(p)) {
      return true;
    }
  }
  return false;
}


void TrafficClass::setQueue(std::queue<Ptr<Packet>> queue) {

  this->m_queue = queue;
}

/**
 * Enqueue the packet to the queue.
 * @param packet
 * @return
 */
bool TrafficClass::Enqueue(Ptr<Packet> packet) {
    NS_LOG_FUNCTION(this);
  if (m_queue.size() < max_packets) {
    m_queue.push(packet);
    return true;
  }
  return false;
}

/**
 * Dequeue the packet from the queue.
 * @return
 */
Ptr<Packet> TrafficClass::Dequeue() {
    NS_LOG_FUNCTION(this);
  Ptr<Packet> packet = m_queue.front();
  m_queue.pop();
  return packet;
}

std::queue<Ptr<Packet>> TrafficClass::getQueue() {
  return m_queue;
}

bool TrafficClass::IsEmpty() {
  return m_queue.empty();
}

Ptr<Packet> TrafficClass::Peek() {
  return m_queue.front();
}

bool TrafficClass::getIsDefault() {
  return isDefault;
}

