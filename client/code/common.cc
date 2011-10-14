/*
 * common.cc
 *
 *  Created on: 21.09.2011
 *      Author: demmeln
 */

#include "common.h"
#include <cmath>
#include <limits>
#include <boost/random.hpp>
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


bool gTimeout = false;


void reset_timeout_flag() {
	gTimeout = false;
}

void timeout_handler_soft (int i) {
	gTimeout = true;
}

void check_timeout() {
	if(gTimeout) {
		throw timeout_exception();
	}
}

void disableTimer() {
	setitimer(ITIMER_REAL, 0, 0);
}

int gTimeoutVal = 0;

void setTimeoutVal(int val) {
	gTimeoutVal = val;
}

int getTimeoutVal() {
	return gTimeoutVal;
}

void enableTimer(uint seconds) {
	struct itimerval diff;
    diff.it_interval.tv_sec = 0;
    diff.it_interval.tv_usec = 0;
    diff.it_value.tv_sec = seconds;
    diff.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &diff, 0);
}

void timeout_handler_hard (int i) {
	cout << "[timeout] exiting..." << endl;
	cout.flush();
	exit(-3);
}


//
//struct Rand64 {
//
//	Rand64() :
//		rd(),
//		engine(), //(rd()),
//		dist(0, numeric_limits<uint64_t>::max())
//	{
//		std::cout << numeric_limits<uint64_t>::max() << std::endl;
//		rnd = std::bind(dist, engine);
//	}
//
//	uint64_t rand() {
//		return rnd();
//	}
//
//private:
//	std::random_device rd;
//	std::mt19937 engine;
//	std::uniform_int_distribution<uint64_t> dist;
//	std::variate_generator<std::mt19937&, boost::uniform_int<uint64_t> > gen64;
//	std::function<uint64_t()> rnd;
//};
//
//Rand64 gRand64;
//
//uint64_t rand64() {
//	return gRand64.rand();
//}

};
