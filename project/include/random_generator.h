#ifndef RAND_GEN
#define RAND_GEN

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

const int MAX_RANDOM_RANGE = 1000000000;

class RandomGenerator
{
public:
	RandomGenerator(int seed);
	~RandomGenerator();
	double Randomize();
	

private:
	boost::random::mt19937* gen;
	boost::random::uniform_int_distribution<>* dist;
};

#endif
