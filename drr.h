#ifndef DRR_H
#define DRR_H

// this class is the subclass of DiffServ. It implements the Deficit Round Robin (DRR) scheduling algorithm.

#include "diff_serv.h"
#include "ns3/packet.h"
#include <vector>

namespace ns3{

class Drr : public DiffServ
{
  public:
    // Drr(std::vector<TrafficClass*> q_class);
    Drr();
    ~Drr();

    static TypeId GetTypeId(void);

    bool Enqueue(Ptr<Packet> p);
    Ptr<Packet> Dequeue(void);
    Ptr<Packet> Remove(void);
    Ptr<const Packet> Peek(void) const;

    std::vector<TrafficClass*> GetTrafficClasses(void);
    void SetTrafficClasses(std::vector<TrafficClass*> q_class);

    //Deficit counter vector methods
    void InitializeDeficitCounter(void);
    std::vector<uint32_t> GetDeficitCounter(void);
    void SetDeficitCounter(std::vector<uint32_t> deficitCounter);

    //Default quantum methods
    uint32_t GetDefaultQuantum(void);
    void SetDefaultQuantum(uint32_t quantum);




  private:
    //Deficit counter vector
    std::vector<uint32_t> m_deficitCounter;
    //Active list index vector
    std::vector<uint32_t> m_activeList;
    //Defualt quantum
    uint32_t m_defaultQuantum;
    //Current traffic class index
    int m_currentIndex;


    //Active list private methods
    bool isActiveListed(uint32_t index);

};
} // namespace ns3

#endif //DRR_H
