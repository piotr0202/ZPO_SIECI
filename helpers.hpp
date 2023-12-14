#ifndef NETSIM_HELPERS_HPP
#define NETSIM_HELPERS_HPP

#include <functional>
#include <random>

#include "types.hpp"

// Inicjalizacja random_device i mt19937
// rd to obiekt zrodlo losowsci
// rng sluzy do generowania konkretnych liczb losowych
// Zostana wykorzystane w pliku cpp
extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //NETSIM_HELPERS_HPP