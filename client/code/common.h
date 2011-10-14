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
#include <exception>
#include <iostream>
#include <cstdlib>
#include <signal.h>

#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <time.h>
#else
#include <sys/time.h>
#endif



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



// TIMEOUT
class timeout_exception: public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Timeout";
	}
};

void setTimeoutVal(int val);
int getTimeoutVal();

void reset_timeout_flag();
void timeout_handler_soft (int i);
void check_timeout();
void timeout_handler_hard (int i);

void disableTimer();

void enableTimer(uint seconds) ;


};

#endif /* COMMON_H_ */
