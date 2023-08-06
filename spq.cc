#include "spq.h"
#include "ns3/packet.h"
#include "diff_serv.h"
#include "traffic_class.h"
#include "ns3/log.h"


/**
 * Spq class is the subclass of DiffServ class. It implements the Strict Priority Queueing algorithm.
 * It is a priority queueing algorithm that services the highest priority queue first.
 * It takes the priority of the queue from the TrafficClass object.
 * And these parameters are given with the config file to the program.
 */
using namespace ns3;
NS_OBJECT_ENSURE_REGISTERED(Spq);
NS_LOG_COMPONENT_DEFINE("Spq");

Spq::Spq() : DiffServ() {

    //Add logger
    LogComponentEnable ("Spq", LOG_LEVEL_ALL);
}

Spq::~Spq() {}

TypeId Spq::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::Spq")
                            .SetParent<DiffServ>()
                            .AddConstructor<Spq>()

        ;
    return tid;
}

/**
 * Set the TrafficClass vector to the Spq object.
 * @param q_class
 */
void Spq::SetTrafficClasses(std::vector<TrafficClass*> q_class) {

    DiffServ::SetTrafficClasses(q_class);
}

/**
 * Get the TrafficClass vector from the Spq object.
 * @return
 */
std::vector<TrafficClass*> Spq::GetTrafficClasses(void) {
    // Return the vector of TrafficClass
    return DiffServ::GetTrafficClasses();
}

/**
 * Classify the packet to the corresponding queue.
 * Gets the index from the DiffServ's class classifier.
 * Using the DoEnqueue function, enqueue the packet to the corresponding queue.
 * @param p
 * @return return true if the packet is classified successfully.
 */
bool Spq::Enqueue(Ptr<ns3::Packet> p){
    // Classify the packet and enqueue it to the corresponding queue.
    // Return true if the packet is enqueued successfully.
    // Return false if the packet is not enqueued.
    int queueIndex = Classify(p);

    if (queueIndex == -1){
        return false;
    }

    return DoEnqueue(queueIndex, p);
}

/**
 * Dequeue the packet from the highest priority non-empty queue.
 * @return packet
 */
Ptr<Packet> Spq::Dequeue(){

    uint32_t queueIndex = 0;
    for (uint32_t i = 0; i < GetTrafficClasses().size (); i++)
    {
        std::vector<TrafficClass*> q_class = GetTrafficClasses();
        if (!q_class[i]->IsEmpty()){
            queueIndex = i;
            return DoDequeue(queueIndex);
        }
    }

    return 0;
}

Ptr<Packet> Spq::Remove(){
    return nullptr;
}

Ptr<const Packet> Spq::Peek() const{
    return nullptr;
}


