#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <list>

enum class PackageQueueType {
    FIFO,
    LIFO
};

// Klasa abstrakcyjna
class IPackageStockpile {
public:
    // Tworzenie const iteratora
    using const_iterator = std::list<Package>::const_iterator;

    // Wirtualna metoda push - wszystkie metody są wirtualne, bo to klasa abstrakcyjna!
    virtual void push(Package&& package) = 0;

    // Metoda sprawdzająca liczbe półproduktów w magazynie
    virtual std::size_t size() const = 0;

    // Metoda sprawdzająca czy magazyn jest pusty
    virtual bool if_empty() const = 0;

    // Iteratory...
    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    // Klasa abstrakcjna zatem definiujemy destruktor
    virtual ~IPackageStockpile() = default;
};

// Klasa abstrakcyjna dziedzicząca po klasie abstrakcyjnej
class IPackageQueue : public IPackageStockpile {
public:
    // Metoda służąca do "wyciągania" półproduktu z kolejki
    virtual Package pop_from_queue() = 0;

    // Metoda służąca do sprawdzania typu kolejki
    virtual PackageQueueType get_queue_type() = 0;

    // Dziedziczymy po klasie abstrakcyjnej, więc należy zdefiniować destruktor
    ~IPackageQueue() override = default;
};

// Klasa wykorzystująca interfejs IPackageQueue
class PackageQueue : public IPackageQueue {
public:
    // W konstrukcji tworzymy kolejke i ustalamy typ kolejki
    PackageQueue(PackageQueueType package_queue_type) : queue_(), package_queue_type_(package_queue_type) {}

    // Wkładanie półproduktu na koniec!
    void push(Package&& package) override { queue_.emplace_back(std::move(package)); }

    std::size_t size() const override { return queue_.size(); }

    bool empty() const override { return queue_.empty(); }

    // Iteratory...
    const_iterator cbegin() const override { return queue_.cbegin(); }

    const_iterator cend() const override { return queue_.cend(); }

    const_iterator begin() const override { return queue_.cbegin(); }

    const_iterator end() const override { return queue_.cend(); }

    // Zależy od typu - zaimplementowane w pliku cpp
    Package pop() override;

    PackageQueueType get_queue_type() const override { return package_queue_type_; }

    ~PackageQueue() override = default;
private:
    std::list<Product> queue_;
    PackageQueueType package_queue_type_;
};

#endif