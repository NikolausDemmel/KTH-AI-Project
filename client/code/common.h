/*
 * common.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef COMMON_H____
#define COMMON_H____

#include <stdint.h>
#include <string>
#include <sstream>
#include <cassert>
#include <boost/foreach.hpp>
#include <limits>
#include "definitions.h"
#include <iostream>



// Make Eclipse parser happy
#ifdef __CDT_PARSER__
    #define foreach(a, b) for(a : b)
#else
    #define foreach BOOST_FOREACH
#endif



using namespace std;


namespace mnp {

enum SearchType {
		Forward 	= 0,
		Backward	= 1 << 0
	};

Dir invertDirection(Dir dir);
char directionToAction(Dir dir);

uint64_t rand64();

};

#endif /* COMMON_H_ */
