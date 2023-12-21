

#include <stdexcept>
#include "package.hpp"
#include "storage_types.hpp"
std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

void insert_if_not_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) == s.end()) {
        s.insert(e);
    }
}

void erase_if_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) != s.end()) {
        s.erase(e);
    }
}

Package::Package() {
    if (freed_IDs.empty()){
        if (assigned_IDs.empty()) id_ = 1;
        else id_ = *assigned_IDs.end() + 1;
    }
    else {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(id_);
    }
    assigned_IDs.insert(id_);
}

Package::Package(ElementID m) {
    if (assigned_IDs.find(m) != assigned_IDs.end()) {
        throw std::invalid_argument("The ID of " + std::to_string(m) + " is already assigned!");
    }

    id_ = m;
    assigned_IDs.insert(id_);
    erase_if_exists(freed_IDs, m);
}

Package::Package(Package&& other) noexcept {
    id_ = other.id_;
    other.id_ = BLANK_ID;
}

Package& Package::operator=(Package&& other) noexcept {

    if (id_ != BLANK_ID) {
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }

    id_ = other.id_;
    other.id_ = BLANK_ID;

    return *this;
}

Package::~Package() {
    if (id_ != BLANK_ID) {
        insert_if_not_exists(freed_IDs, id_);
        erase_if_exists(assigned_IDs, id_);
    }
}


