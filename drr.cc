#include "drr.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"

/**
 * Drr class is the implementation of Deficit Round Robin (DRR) algorithm. It is a subclass of DiffServ class.
 *
 */
using namespace ns3;
NS_OBJECT_ENSURE_REGISTERED(Drr);
NS_LOG_COMPONENT_DEFINE("Drr");

Drr::Drr() : DiffServ() {

    LogComponentEnable ("Drr", LOG_LEVEL_ALL);
    m_currentIndex = -1;

}



Drr::~Drr() {}

TypeId Drr::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::Drr")
                            .SetParent<DiffServ>()
                            .AddConstructor<Drr>()

        ;
    return tid;
}


/**
 * Set the default quantum for DRR algorithm
 * @param quantum
 */
void Drr::SetDefaultQuantum(uint32_t quantum)
{
    m_defaultQuantum = quantum;
}

/**
 * Initalizing the deficit counters for each queue
 */
void Drr::InitializeDeficitCounter()
{

    std::vector<TrafficClass*> q_class = GetTrafficClasses();
    uint32_t size = q_class.size();
    m_deficitCounter.resize(size);

    for (uint32_t i = 0; i < size; i++)
    {
        m_deficitCounter[i] = 0;
    }

}

/**
 * Set the vector of TrafficClass for DRR algorithm
 * @param q_class
 */
void Drr::SetTrafficClasses(std::vector<TrafficClass*> q_class) {

    DiffServ::SetTrafficClasses(q_class);
}


/**
 * Get the vector of TrafficClass for DRR algorithm
 * @return vector of TrafficClass
 */
std::vector<TrafficClass*> Drr::GetTrafficClasses(void) {
    // Return the vector of TrafficClass
    return DiffServ::GetTrafficClasses();
}

/**
 * Using DiffServ'c classify function to get the index of the queue that the packet belongs to. Then calls DoEnqueue function to enqueue the packet.
 * @param p
 * @return
 */
bool Drr::Enqueue(Ptr<Packet> p)
{

    int queueIndex = Classify(p);

    if (queueIndex == -1)
    {
        return false;
    }

    if(!isActiveListed( queueIndex)){
        //insert queueIndex into active list
        m_activeList.push_back(queueIndex);
        //initialize deficit counter
        m_deficitCounter[queueIndex] = 0;
    }

    return DoEnqueue(queueIndex, p);

}

/**
 * Check if the queueIndex is in the active list
 * @param queueIndex
 * @return true if the queueIndex is in the active list, false otherwise
 */
bool Drr::isActiveListed(uint32_t queueIndex)
{
    for (uint32_t i = 0; i < m_activeList.size(); i++)
    {
        if (m_activeList[i] == queueIndex)
        {
            return true;
        }
    }

        return false;
}


/**
 * Dequeue the packet from the queue that has the highest priority.
 * If the packet size is smaller than the deficit counter, dequeue the packet and update the deficit counter.
 * If the packet size is bigger than the deficit counter, erase the queueIndex from active list, and move it to the end of the active list.
 * If the queue is empty, erase the queueIndex from active list. If the active list is empty, return 0.
 * @return packet
 */
Ptr<Packet> Drr::Dequeue()
{
    Ptr<Packet> p = nullptr;

    while (m_activeList.size() != 0)
    {
        int queueIndex = m_activeList.front();
        if (m_currentIndex != -1 && m_currentIndex == queueIndex)
        {
            // if the current index is the same as the queueIndex, we do not need to update the deficit counter
        }
        else
        {
            m_currentIndex = queueIndex;
            double weight = GetTrafficClasses()[queueIndex]->getWeight();
            double quantum = m_defaultQuantum * weight;
            m_deficitCounter[queueIndex] += quantum;

        }

        while (m_deficitCounter[queueIndex] > 0 && !GetTrafficClasses()[queueIndex]->IsEmpty())
        {
            uint32_t packetSize = GetTrafficClasses()[queueIndex]->Peek()->GetSize();

            if (packetSize <= m_deficitCounter[queueIndex])
            {
                m_deficitCounter[queueIndex] -= packetSize;
                p = DiffServ::DoDequeue(queueIndex);
                return p;
            }
            else
            {
                // this means that the packet is bigger than the deficit counter, erase the queueIndex from active list, and move to the next queue.
                m_activeList.erase(m_activeList.begin());
                // change the current index to -1
                m_currentIndex = -1;
                break;
            }
        }

        // this means that the queue is empty, erase the queueIndex from active list,
        if (GetTrafficClasses()[queueIndex]->IsEmpty())
        {
            m_deficitCounter[queueIndex] = 0;
            // erase queueIndex from active list
            m_activeList.erase(m_activeList.begin());
            // change the current index to -1
            m_currentIndex = -1;
        }
        else
        {
            // insert queueIndex into active list
            m_activeList.push_back(queueIndex);
            m_currentIndex = -1;

        }

        if (m_activeList.size() == 0)
        {
            // std::cout << "Active list is empty" << std::endl;
            break;
        }
    }

    return 0;
}


Ptr<Packet> Drr::Remove()
{
    return nullptr;
}

Ptr<const Packet> Drr::Peek() const
{
    return nullptr;
}

















