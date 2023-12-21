
#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <set>
#include "types.hpp"
//class Package {
//    + Package()
//    + Package(ElementID)
//    + Package(Package&&)
//    + operator=(Package&&) : Package&
//            + get_id(): ElementID {query}
//            + ~Package()
//}

class Package{
public:
    Package();

    explicit Package(ElementID m);


    Package(const Package&) = delete;
    Package(Package&&) noexcept;

    Package& operator=(Package&) = delete;
    Package& operator=(Package&&) noexcept;


    [[nodiscard]] const ElementID& get_id() const {return id_;};

    ~Package();
private:
    static const ElementID BLANK_ID = -1;

    ElementID id_ = BLANK_ID;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //NETSIM_PACKAGE_HPP
