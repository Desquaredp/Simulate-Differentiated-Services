#ifndef DIFF_SERV_H
#define DIFF_SERV_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"

#include <vector>
#include "traffic_class.h"

namespace ns3 {
// Header Class for diff_serv.cc
class DiffServ : public Queue<Packet>
{
  public:
    DiffServ();
    virtual ~DiffServ();

    Ptr<Packet> Schedule(void);
    int Classify(Ptr<Packet> p);

    std::vector<TrafficClass*> GetTrafficClasses(void) ;
    void SetTrafficClasses(std::vector<TrafficClass*> q_class) ;

  protected:
    bool DoEnqueue(u_int pos, Ptr<Packet> p);
    Ptr<Packet> DoDequeue(u_int pos);


  private:
    std::vector<TrafficClass*> q_class;

};

} // namespace ns3

#endif //DIFF_SERV_H