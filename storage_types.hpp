
//Piotr Kwapień, Paweł Kocur Antoni Krempa

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <cstddef>
#include "package.hpp"
#include <string>
#include <list>
#include <deque>


enum class PackageQueueType{
    FIFO, LIFO
};


class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& pck) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

};


class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue: public IPackageQueue{
private:
    PackageQueueType mQueueType;
    std::list<Package> mQueue;
public:
    PackageQueue(PackageQueueType queueType) : mQueueType(queueType) {}
    PackageQueueType get_queue_type() const override { return mQueueType; }

    Package pop() override;
    void push(Package&& pck) override;
    bool empty() const override     { return mQueue.empty();  }
    size_t size() const override    { return mQueue.size();   }
    const_iterator begin() const override { return mQueue.begin();  }
    const_iterator end() const override   { return mQueue.end();    }
    const_iterator cbegin()const override { return mQueue.cbegin(); }
    const_iterator cend() const override  { return mQueue.cend();   }
};

#endif //NETSIM_STORAGE_TYPES_HPP

