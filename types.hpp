#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP


#include <list>
#include <functional>
#include "package.hpp"

using size_type=std::size_t;
// Dla pewności użyjemy "wiekszego" typu - zamiast int long long
using ElementID = unsigned long long;
using TimeOffset = unsigned long long; 
using Time = unsigned long long;
using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HPP
