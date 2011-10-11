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

const char* dirToString(Dir dir)
{
	switch(dir) {
	case Up:
		return "Up";
	case Down:
		return "Down";
	case Left:
		return "Left";
	case Right:
		return "Right";
	default:
		throw "Unreachable!";
	}
}

const char* searchResultToString(SearchResult result)
{
	switch(result) {
	case Solution:
		return "Solution";
	case Failure:
		return "Failure";
	case CutOff:
		return "CutOff";
	default:
		throw "Unreachable!";
	}
}

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

	// FIXME: seed?

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
