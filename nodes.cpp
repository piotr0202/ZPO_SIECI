
#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    preferences_.emplace(std::make_pair(r,1.0)); // get_random()
    rebuild_pref();
}
void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    preferences_.erase(r);
    rebuild_pref();
}
IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double number = mRng();
    double sum = 0;
    for (const auto &item: preferences_)
    {
        sum += item.second;
        if(number <= sum)
            return item.first;
    }
    return preferences_.end()->first;
}
void ReceiverPreferences::rebuild_pref()
{
    double sum = 0;

    for(auto& elem : preferences_){
        elem.second = 1;
        sum+=1;
    }

    for(auto& elem : preferences_){
        elem.second=1/sum;
    }
}
void PackageSender::send_package()
{
    if(mBuffer)
    {
        receiver_preferences_.choose_receiver()->receive_package(std::move(*mBuffer));
        //std::cout<<"Co bylo w buforze: "<< (*mBuffer).get_id()<<std::endl;
        mBuffer.reset();
    }
}
void PackageSender::push_package(Package&& pck)
{
    mBuffer.emplace(std::move(pck));
}
void Ramp::deliver_goods(Time t)
{
    if(t % mOffset == 1)
    {
        Package pkc;
        push_package(std::move(pkc));
        //send_package();
    }
}
Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> queue_ptr)
{
    mID = id; mOffset = pd;
    mUniquePtr = std::move(queue_ptr);
}
void Worker::do_work(Time t)
{
    if((t-mTime)% mOffset - 1 == 0)
    {
        if(mWorkerBuffer)
        {
            push_package(std::move(*mWorkerBuffer));
            mWorkerBuffer.reset();
        }
        mWorkerBuffer.emplace(mUniquePtr->pop());
        //send_package();
        mTime = t;
    }


}
Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d)
{
    mID = id;
    mUniquePtr = std::move(d);
}

void Storehouse::receive_package(Package&& p)
{
    mUniquePtr->push(std::move(p));
}

void Worker::receive_package(Package&& pck)
{
    mUniquePtr->push(std::move(pck));

}