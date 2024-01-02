
#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP
#include "nodes.hpp"
#include <iostream>
#include <sstream>
#include <vector>

enum class NodeColor{
    UNVISITED, VISITED, VERIFIED
};

enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

//tworzenie szablonu klasy
template <typename Node>
class NodeCollection{
public:

    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    // Prawie tak, jak w konspekcie
    NodeCollection<Node>::iterator find_by_id(ElementID id_) { return std::find_if(mContainer.begin(), mContainer.end(), [id_](const auto& elem){ return id_ == elem.get_id();});}
    NodeCollection<Node>::const_iterator find_by_id(ElementID id_) const { return std::find_if(mContainer.cbegin(), mContainer.cend(), [id_](const auto& elem){ return id_ == elem.get_id();}); }

    // Wypisalem wszystkie mozliwe (mialobyc do 6 wiec jest 6)
    iterator begin(){ return mContainer.begin(); }
    iterator end(){ return mContainer.end(); }
    const_iterator begin() const{ return mContainer.cbegin(); }
    const_iterator cbegin() const{ return mContainer.cbegin(); }
    const_iterator end() const{ return mContainer.cend(); }
    const_iterator cend() const{ return mContainer.cend(); }

    void add(Node& node) { mContainer.emplace_back(std::move(node));}
    void remove_by_id(ElementID id_)
    {
        auto it = find_by_id(id_);
        if(it != mContainer.end())
        {
            mContainer.erase(it);
        }
    }
private:
    container_t mContainer;
};

class Factory{
public:
    void add_ramp(Ramp&& r){Ramps.add(r);}
    void remove_ramp(ElementID id){Ramps.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return Ramps.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin()const{return Ramps.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend()const{return Ramps.cend();}

    void add_worker(Worker&& w){Workers.add(w);}
    void remove_worker(ElementID id){this -> remove_receiver(Workers,id);}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return Workers.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin()const{return Workers.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend()const{return Workers.cend();}

    void add_storehouse(Storehouse&& s){Storehouses.add(s);}
    void remove_storehouse(ElementID id){this -> remove_receiver(Storehouses,id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return Storehouses.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin()const{return Storehouses.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend()const{return Storehouses.cend();}

    bool is_consistent();
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);
private:
    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {
        std::for_each(Ramps.begin(), Ramps.end(), [id](Ramp &ramp)
        {
            auto ramp_receivers = ramp.receiver_preferences_.get_preferences();

            for (auto receiver = ramp_receivers.begin(); receiver != ramp_receivers.end(); receiver++) {

                if(receiver->first->get_id()==id) {
                    ramp.receiver_preferences_.remove_receiver(receiver->first);
                }
            }
        });

        std::for_each(Workers.begin(), Workers.end(), [id](Worker &worker) {
            auto worker_receivers = worker.receiver_preferences_.get_preferences();

            for (auto receiver = worker_receivers.begin(); receiver != worker_receivers.end(); receiver++) {

                if(receiver->first->get_id()==id) {
                    worker.receiver_preferences_.remove_receiver(receiver->first);
                }
            }

        });
        collection.remove_by_id(id);
    }
    NodeCollection<Ramp> Ramps;
    NodeCollection<Worker> Workers;
    NodeCollection<Storehouse> Storehouses;
};

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

// Oprócz powyższych funkcji głównych potrzebne są także następujące funkcje i structy
struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string line);

#endif //NETSIM_FACTORY_HPP

//Piotr Kwapień, Paweł Kocur, Antoni Krempa