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

#include "definitions.h"

#define DEBUG
#define INFO
#define VERBOSE_SHORTEST_PATH
// #define VERBOSE_GENERATE_MOVES


// TODO: Use boost, e.g. for foreach

// IDEA: store a list of indeces, that are actually relevat!!!


using namespace std;

namespace mnp {


enum Dir {
	Up = 0,
	Left = 1,
	Down = 2,
	Right = 3
};



class Pos
{
public:
	Pos(coord_t x_, coord_t y_) {
		x = x_;
		y = y_;

	}

	Pos() {
		x = 0;
		y = 0;
	}

	Pos(Pos currentPos, Dir offset, int m=1) {
		x = currentPos.x;
		y = currentPos.y;
		switch (offset) {
		case Up:
			y+=1*m;
			break;
		case Down:
			y-=1*m;
			break;
		case Left:
			x-=1*m;
			break;
		case Right:
			x+=1*m;
			break;
		}
	}

	string ToString() const {
		stringstream ss;
		ss << "(" << x << "," << y << ")";
		return ss.str();
	}


	coord_t x, y;
};


enum SearchResult {
	Solution,
	Failure,
	CutOff
};


// TODO: find proper place for this
struct TileNode {

	TileNode():
		distance(numeric_limits<int>::max()),
		visited(false),
		parent(Up)
	{
	}

	int distance;
	bool visited;
	Dir parent;

};


const char* DirToString(Dir dir);
const char* SearchResultToString(SearchResult result);
Dir invertDirection(Dir dir);
char directionToAction(Dir dir);

inline bool isTileFree(uint8_t tile)
{
	tile &= ~TileFlagMask;
	return (tile == TileEmpty) || ((tile & TileGoal) && !(tile & TileBox));
}

uint64_t rand64();

};

#endif /* COMMON_H_ */
