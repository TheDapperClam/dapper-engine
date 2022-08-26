#include "Random.h"
#include <random>
#include <ctime>

const bool dap::Random::WAS_SEEDED = Seed ( std::time ( NULL ) );

float dap::Random::Range ( float min, float max ) {
	return min + ( static_cast<float>(std::rand ()) / static_cast<float>(RAND_MAX / (max - min)) );
}

int dap::Random::Range ( int min, int max ) {
	return min + ( std::rand () % ( max - min + 1 ) );
}

bool dap::Random::Seed ( unsigned int seed ) {
	srand ( seed );
	return true;
}
