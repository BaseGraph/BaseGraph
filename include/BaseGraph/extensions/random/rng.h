#ifndef BASE_GRAPH_RNG_H
#define BASE_GRAPH_RNG_H


#include <random>


namespace BaseGraph { namespace random {

extern std::mt19937_64 rng;
inline void seed(size_t seed_) { rng.seed(seed_); }

}} // namespace BaseGraph::random

#endif
