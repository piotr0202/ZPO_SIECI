#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& package) = 0;

    virtual std::size_t size() const = 0;

    virtual bool empty() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;
};

#endif