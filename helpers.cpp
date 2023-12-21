
#include "helpers.hpp"
#include <cstdlib>
#include <random>

std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}

std::function<double()> probability_generator = default_probability_generator;
/*
double get_random() {
    return std::generate_canonical<double, 10>(rng);
}
*/
double your_num() {
    return 0.2;
}
