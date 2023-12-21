
#include <factory.hpp>



bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if(node_colors[sender] == NodeColor::VERIFIED)
    {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if(sender->receiver_preferences_.preferences_.empty())
    {
        throw std::logic_error("ERROR::NETWORK::INCONSISTENT");
    }

    bool is_there_anyone_in_here = false;
    for(auto& receiver: sender->receiver_preferences_.preferences_)
    {
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
        {
            is_there_anyone_in_here = true;
        } else
        {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrecv_ptr != sender) is_there_anyone_in_here = true;
            if(node_colors[sendrecv_ptr] != NodeColor::VISITED)
            {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if(is_there_anyone_in_here) return true;
    else throw std::logic_error("ERROR::NETWORK::INCONSISTENT");
}

bool Factory::is_consistent()
{
    std::map<const PackageSender*, NodeColor> nodeColor;
    for(auto& worker: Workers) nodeColor.emplace(std::make_pair(&worker, NodeColor::UNVISITED));
    for(auto& ramp: Ramps) nodeColor.emplace(std::make_pair(&ramp, NodeColor::UNVISITED));

    for(const auto& sender: Ramps)
    {
        try
        {
            has_reachable_storehouse(&sender, nodeColor);
        }
        catch(const std::logic_error& )
        {
            return false;
        }
    }
    return true;
}


void Factory::do_deliveries(Time time)
{
    //std::for_each(Ramps.begin(),Ramps.end(),[](auto &ramps){ramps.deliver_goods(time);});
    for(auto& ramps: Ramps)
    {
        ramps.deliver_goods(time);
    }
}

void Factory::do_package_passing()
{
    for(auto& ramp: Ramps)
    {
        if(ramp.get_sending_buffer().has_value())
        {
            ramp.send_package();
        }
    }
    for(auto& worker: Workers)
    {
        if(worker.get_sending_buffer().has_value())
        {
            worker.send_package();
        }
    }
}

void Factory::do_work(Time time)
{
    for(auto& worker: Workers)
    {
        worker.do_work(time);
    }
}

// Ta funkcja ma odpowiadac za usuwanie polaczen miedzy dostawcami

/*
template <typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    std::for_each(Ramps.begin(), Ramps.end(), [id](Ramp &ramp) {
        auto ramp_receivers = ramp.receiver_preferences_.get_preferences();

        for (auto receiver = ramp_receivers.begin(); receiver != ramp_receivers.end(); receiver++) {
            ramp.receiver_preferences_.remove_receiver(receiver->first);
        }
    });

    std::for_each(Workers.begin(), Workers.end(), [id](Worker &worker) {
        auto worker_receivers = worker.receiver_preferences_.get_preferences();

        for (auto receiver = worker_receivers.begin(); receiver != worker_receivers.end(); receiver++) {
            worker.receiver_preferences_.remove_receiver(receiver->first);
        }
    });
    collection.remove_by_id(id);

*/