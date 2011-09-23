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

#define DEBUG
#define INFO
// #define VERBOSE_GENERATE_MOVES



using namespace std;

namespace mnp {

// TODO: Maybe change to this scheme for maximum (?) space compression:
// 1 bit for floor/wall
// 1 bit for goal/no-goal
// 1 bit for box/no-box
// rest of the bits for flags
// invalid could be represented as some senseless combination, eg. wall & box
// test for "free to move to" would be something like (wall | box) & my-tile == 0

enum Tile {
	TileEmpty		= 1 << 0,
	TileWall		= 1 << 1,
	TileGoal		= 1 << 2,
	TileBox			= 1 << 3,
	TileVisitedFlag	= 1 << 4,
	TileExtraFlag	= 1 << 5
};

static const uint8_t TileFlagMask = TileVisitedFlag | TileExtraFlag; // should be logical or of all flag bits.

enum Dir {
	Up = 0,
	Left = 1,
	Down = 2,
	Right = 3
};

typedef unsigned int coord_t;

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

const char* DirToString(Dir dir);
Dir invertDirection(Dir dir);
char directionToAction(Dir dir);

inline bool isTileFree(uint8_t tile)
{
	tile &= ~TileFlagMask;
	return (tile == TileEmpty) || ((tile & TileGoal) && !(tile & TileBox));
}

};

#endif /* COMMON_H_ */
