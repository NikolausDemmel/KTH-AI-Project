/*
 * definitions.h
 *
 *  Created on: 10.10.2011
 *      Author: demmeln
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


#include <stdint.h>


////////////////////////////////////////////////////////////////////////////////
// Symbols controlling level of debugging
////////////////////////////////////////////////////////////////////////////////

// #define NDEBUG // < define this to disable assertions etc. Only use to get the last bit of speed.
#define DEBUG
#define INFO
// #define VERBOSE_SHORTEST_PATH
// #define VERBOSE_GENERATE_MOVES


////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////
using namespace std;


namespace mnp {


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
typedef unsigned int coord_t;
typedef unsigned int index_t;
typedef unsigned int uint;
typedef const char * cstring_t;


////////////////////////////////////////////////////////////////////////////////
// Enumeration types
////////////////////////////////////////////////////////////////////////////////
enum Dir {
	Up = 0,
	Left,
	Down,
	Right
};

enum SearchResult {
	Solution = 0,
	SolutionMeeting,
	Failure,
	CutOff
};

const Dir cDirs[] = { Up, Left, Down, Right };
const cstring_t cDirNames[] = { "Up", "Left", "Down", "Right" };

const SearchResult cSearchResults[] = { Solution, SolutionMeeting, Failure, CutOff };
const cstring_t cSearchResultNames[] = { "Solution", "SolutionMeeting", "Failure", "CutOff" };


}


#endif /* DEFINITIONS_H_ */
