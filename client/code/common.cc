/*
 * common.cc
 *
 *  Created on: 21.09.2011
 *      Author: demmeln
 */

#include "common.h"
#include <cmath>
#include <limits>
#include <random>
#include <functional>


namespace mnp {



Dir invertDirection(Dir dir) {
	switch(dir) {
	case Up:
		return Down;
	case Down:
		return Up;
	case Left:
		return Right;
	case Right:
		return Left;
	}
}

char directionToAction(Dir dir) {
	switch(dir) {
	case Up:
		return 'U';
	case Down:
		return 'D';
	case Left:
		return 'L';
	case Right:
		return 'R';
	}
}

struct Rand64 {

	Rand64() :
		rd(),
		engine(rd()),
		dist(0, numeric_limits<uint64_t>::max())
	{
		rnd = std::bind(dist, engine);
	}

	uint64_t rand() {
		return rnd();
	}

private:
	std::random_device rd;
	std::mt19937 engine;
	std::uniform_int_distribution<uint64_t> dist;
	std::function<uint64_t()> rnd;
};

Rand64 gRand64;

uint64_t rand64() {
	return gRand64.rand();
}

};
