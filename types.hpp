
#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <cstdint>
#include <deque>
#include <functional>

using ElementID = int;
using Time = uint64_t;
using TimeOffset = uint64_t;
using ProbabilityGenerator = std::function<double(void)>;

#endif //NETSIM_TYPES_HPP
