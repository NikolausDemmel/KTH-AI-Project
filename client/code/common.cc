/*
 * common.cc
 *
 *  Created on: 21.09.2011
 *      Author: demmeln
 */

#include "common.h"
#include <cmath>
#include <limits>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>


namespace mnp {

const char* DirToString(Dir dir)
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

const char* SearchResultToString(SearchResult result)
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
		gen(),
		dist(0, numeric_limits<uint64_t>::max()),
		gen64(gen, dist)
	{
	}

	uint64_t rand() {
		return gen64();
	}
private:
	boost::mt19937 gen;
	boost::uniform_int<uint64_t> dist;
	boost::variate_generator<boost::mt19937&, boost::uniform_int<uint64_t> > gen64;
};

Rand64 gRand64;

uint64_t rand64() {
	return gRand64.rand();
}

};
