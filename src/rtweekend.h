#pragma once

#include <limits>
//#include <cstdlib>
#include <random>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double less_zeroish = 1e-8;
const double zeroish = 1e-160;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/*
inline double random_double() {
    // Return a random real in [0, 1)
    return std::rand() / (RAND_MAX + 1.0);
}
*/

inline double random_double() {
    // Return a random real in [0, 1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Return a random real in [min, max)
    return min + ((max - min) * random_double());
}

inline int random_int(int min, int max) {
    // Return a random integrer in [min, max].
    return int(random_double(min, max + 1));
}
