#ifndef SPQ_H
#define SPQ_H

#include "diff_serv.h"
#include "ns3/packet.h"
#include <vector>

using namespace ns3;

class Spq : public DiffServ {

  public:

    Spq();
    ~Spq();

    static TypeId GetTypeId(void);

    bool Enqueue(Ptr<Packet> p);
    Ptr<Packet> Dequeue(void);
    Ptr<Packet> Remove(void);
    Ptr<const Packet> Peek(void) const;


    std::vector<TrafficClass*> GetTrafficClasses(void);
    void SetTrafficClasses(std::vector<TrafficClass*> q_class);



};


#endif //SPQ_H