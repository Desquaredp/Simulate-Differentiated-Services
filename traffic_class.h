#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "ns3/packet.h"
#include "ns3/queue.h"
#include <string>
#include <vector>
#include <queue>
#include "filter_element.h"
#include "filter.h"



namespace ns3 {

class TrafficClass
{
  public:
    TrafficClass();

    void setName(std::string name);
    std::string getName();

    void setPackets(uint32_t num);
    uint32_t getPackets();

    void setMaxPackets(uint32_t num);
    uint32_t getMaxPackets();


    void setWeight(double num);
    double getWeight();

    void setPriorityLevel(uint32_t num);
    uint32_t getPriorityLevel();

    bool match(Ptr<Packet> p);

    void setFilters(std::vector<Filter*> filters);
    void addFilter(Filter* filter);
    std::vector<Filter*> getFilters();

    void setQueue(std::queue<Ptr<Packet>> queue);
    bool Enqueue(Ptr<Packet> packet);
    Ptr<Packet> Dequeue();
    std::queue<Ptr<Packet>> getQueue();
    Ptr<Packet> Peek();
    bool IsEmpty();

    //getters and setters for isDefault
    bool getIsDefault();
    void setIsDefault(bool check);


  private:
    std::string name;
    uint32_t packets;
    uint32_t max_packets;
    double weight;
    uint32_t priority_level;
    bool isDefault;
    std::queue<Ptr<Packet>> m_queue;
    std::vector<Filter*> filters;
};

} // namespace ns3

#endif /* TRAFFIC_CLASS_H */
