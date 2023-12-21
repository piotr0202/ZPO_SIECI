//
//
//
#include "package.hpp"
#include "storage_types.hpp"
//Package PackageQueue::pop(){
//
//    Package popped_element;
//    switch(type_){
//        case PackageQueueType::FIFO :{
//            //get first element
//            popped_element = std::move(queue_.front());
//            //delete popped element
//            queue_.pop_front();
//            break;
//        };
//        case PackageQueueType::LIFO :{
//            //get last element
//            popped_element = std::move(queue_.back());
//            //delete popped element
//            queue_.pop_back();
//            break;
//        };
//    };
//    return popped_element;
//}

Package PackageQueue::pop() {
    Package pck;
    switch(get_queue_type())
    {
        case PackageQueueType::FIFO :
        {
            pck = std::move(mQueue.front());
            mQueue.pop_front();
        }
        break;
        case PackageQueueType::LIFO :
        {
            pck = std::move(mQueue.back());
            mQueue.pop_back();
        }
        break;
    }
    return pck;

}

void PackageQueue::push(Package &&pck) {
    mQueue.push_back(std::move(pck));
}