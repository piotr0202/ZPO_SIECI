#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP

#include <list>

#include "package.hpp"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    // Tworzenie const interatora IPackageStockpile::const_iterator
    using const_iterator = std::list<Package>::const_iterator;


}

#endif