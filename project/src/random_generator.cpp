#include "random_generator.h"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

RandomGenerator::RandomGenerator(int seed)
{
	gen = new boost::random::mt19937(seed);
	dist = new boost::random::uniform_int_distribution<>(0,MAX_RANDOM_RANGE);
}

double RandomGenerator::Randomize()
{
	return ((*dist)(*gen)/(double) MAX_RANDOM_RANGE);
}


