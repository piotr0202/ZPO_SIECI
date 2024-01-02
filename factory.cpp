
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

ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;
    
    std::istringstream token_stream(line);
    char delimiter = ' ';
    
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    ParsedLineData data;
     
    if (tokens.size() == 0) {
        throw std::invalid_argument("blank input");
    }
    else if (tokens[0] == "WORKER") {
        data.element_type = ElementType::WORKER;
    }
    else if (tokens[0] == "RAMP") {
        data.element_type = ElementType::RAMP;
    }
    else if (tokens[0] == "STOREHOUSE") {
        data.element_type = ElementType::STOREHOUSE;
    }
    else if (tokens[0] == "LINK") {
        data.element_type = ElementType::LINK;
    }
    else
        throw std::logic_error("Inappropriate line");

    for (auto each : tokens) {
            if ((each != "WORKER") && (each != "RAMP") && (each != "STOREHOUSE") && (each != "LINK")) {
                std::string kv_token;
                std::vector<std::string> keys_values;
                std::istringstream key_value_stream(each);
                while (std::getline(key_value_stream, kv_token, '=')) {
                    keys_values.push_back(kv_token);
                }
                data.parameters[keys_values[0]] = keys_values[1];
            }
        }
        
    return data;
}

Factory load_factory_structure(std::istream& is) {
    Factory factory;

    std::string line;
 
    while (std::getline(is, line)) {
        if ((line[0] != ';') || (line.empty())) {
            ParsedLineData parsed_line = parse_line(line);
            if (parsed_line.element_type == ElementType::WORKER) {
                if (parsed_line.parameters["queue-type"] == "LIFO") {
                    PackageQueue queue(PackageQueueType::LIFO);
                    factory.add_worker(Worker(std::stoi(parsed_line.parameters["id"]), std::stoi(parsed_line.parameters["processing-time"]), std::make_unique<IPackageQueue>(queue)));
                }
                else {
                    PackageQueue queue(PackageQueueType::FIFO);
                    factory.add_worker(Worker(std::stoi(parsed_line.parameters["id"]), std::stoi(parsed_line.parameters["processing-time"]), std::make_unique<IPackageQueue>(queue)));
                }
            }
            else if (parsed_line.element_type == ElementType::STOREHOUSE) {
                factory.add_storehouse(Storehouse(std::stoi(parsed_line.parameters["id"])));
            }
            else if (parsed_line.element_type == ElementType::RAMP) {
                factory.add_ramp(Ramp(std::stoi(parsed_line.parameters["id"]), std::stoi(parsed_line.parameters["delivery-interval"])));
            }
            else if (parsed_line.element_type == ElementType::LINK) {
                std::vector<std::string> tokens;
                std::string token;
                
                std::istringstream token_stream(line);
                char delimiter = '-';
                
                while (std::getline(token_stream, token, delimiter)) {
                    tokens.push_back(token);
                }

                std::pair<std::string, int> src = std::make_pair(tokens[0], std::stoi(tokens[1]));
                std::pair<std::string, int> dest = std::make_pair(tokens[2], std::stoi(tokens[3]));

                PackageSender * sender;
                if (src.first == "ramp") {
                    sender = &(*factory.find_ramp_by_id(static_cast<ElementID>(src.second)));
                }
                else if (src.first == "worker") {
                    sender = &(*factory.find_worker_by_id(static_cast<ElementID>(src.second)));
                }
                else {
                    throw std::logic_error("Invalid source");
                }

                IPackageReceiver * receiver;
                if (dest.first == "worker") {
                    receiver = &(*factory.find_worker_by_id(static_cast<ElementID>(dest.second)));
                }
                else if (dest.first == "store") {
                    receiver = &(*factory.find_storehouse_by_id(static_cast<ElementID>(dest.second)));
                }
                else {
                    throw std::logic_error("Invalid destination");
                }

                sender->receiver_preferences_.add_receiver(receiver);

            }
        }
    }
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    os << "; == LOADING RAMPS ==\n\n";
    for (auto iter = factory.ramp_cbegin(); iter != factory.ramp_cend(); ++iter) {
        os << "LOADING_RAMP id=" << iter->get_id() << " delivery-interval=" << iter->get_delivery_interval() << "\n";
    }

    os << "\n; == WORKERS ==\n\n";
    for (auto iter = factory.worker_cbegin(); iter != factory.worker_cend(); ++iter) {
        os << "WORKER id=" << iter->get_id() << " processing-time=" << iter->get_processing_duration() << " queue-type="
           << (iter->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO") << "\n"; // potrzeba metody do zwrot rodzaju kolejki
    }

    os << "\n; == STOREHOUSES ==\n\n";
    for (auto iter = factory.storehouse_cbegin(); iter != factory.storehouse_cend(); ++iter) {
        os << "STOREHOUSE id=" << iter->get_id() << '\n';
    }

    os << "\n; == LINKS ==\n\n";
    for (auto iter = factory.ramp_cbegin(); iter != factory.ramp_cend(); ++iter) {
        for (auto &worker: iter->receiver_preferences_.get_preferences()) {
            os << "LINK src=ramp-" << iter->get_id() << " dest=worker-" << worker.first->get_id() << "\n";
        }
        os << '\n';
    }

    for (auto iter = factory.worker_cbegin(); iter != factory.worker_cend(); ++iter) {
        for (auto &receiver: iter->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::WORKER)
                os << "LINK src=worker-" << iter->get_id() << " dest=worker-" << receiver.first->get_id() << '\n';
            else
                os << "LINK src=worker-" << iter->get_id() << " dest=store-" << receiver.first->get_id() << '\n';
        }
        os << '\n';
    }
    os.flush();
}
