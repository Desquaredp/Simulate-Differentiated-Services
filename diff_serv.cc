#include "diff_serv.h"
#include "ns3/log.h"
#include "traffic_class.h"

/**
 * Base class for the differentiated services. Subclass of ns3::Queue class.
 */
using namespace ns3;

DiffServ :: DiffServ ()
    :
      q_class()
{

}

DiffServ :: ~DiffServ ()
{
}

/**
 * Find the index of the TrafficClass that matches the packet p. Return -1 if no match is found and there is no default queue.
 * @param p
 * @return index
 */
int DiffServ :: Classify (Ptr<Packet> p)
{

    int index = -1;

    for (uint32_t i = 0; i < q_class.size (); i++)
    {

        if (q_class[i]->match (p))
        {
            index = i;
            return index;
        }
    }

    if (index == -1){

        //have an isDefault check.
        for (uint32_t i = 0; i < q_class.size (); i++)
        {
            if (q_class[i]->getIsDefault() == true)
            {

                index = i;
                return index;
            }
        }
    }

    return index;

}

/**
 * Enqueue the packet p to the TrafficClass at index pos. Return true if the packet was enqueued successfully, false otherwise.
 * @param pos
 * @param p
 * @return bool
 */
bool DiffServ::DoEnqueue(u_int pos, Ptr<ns3::Packet> p)
{
    // Enqueue the packet to the TrafficClass at index pos. Return true if the packet was enqueued successfully, false otherwise.
    return this->q_class[pos]->Enqueue(p);
}

/**
 * Dequeue the packet from the TrafficClass at index pos. Return the packet.
 * @param pos
 * @return packet
 */
Ptr<Packet> DiffServ::DoDequeue(u_int pos)
{
    // Dequeue the packet from the TrafficClass at index pos. Return the packet.
    return this->q_class[pos]->Dequeue();
}


/**
 * Return the vector of TrafficClass
 * @return
 */
std::vector<TrafficClass*> DiffServ :: GetTrafficClasses (void)
{
    // Return the vector of TrafficClass
    return q_class;
}


/**
 * Set the vector of TrafficClass
 * @param trafficClasses
 */
void DiffServ :: SetTrafficClasses (std::vector<TrafficClass*> trafficClasses)
{
    // Set the vector of TrafficClass
    this->q_class = trafficClasses;
}
