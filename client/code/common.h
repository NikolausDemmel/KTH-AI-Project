/*
 * common.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef COMMON_H____
#define COMMON_H____

namespace mnp {

enum Tile {
	TileEmpty = 0,
	TileWall = 1,
	TileGoal = 1 << 1,
	TileBox = 1 << 2,
	TileVisitedFlag =  1 << 3
};

enum Dir {
	Up,
	Down,
	Left,
	Right
};

class Pos
{
public:
	Pos(uint8_t x_, uint8_t y_) {
		x = x_;
		y = y_;

	}

	Pos() {
		x = 0;
		y = 0;
	}

	uint8_t x, y;
};

};

#endif /* COMMON_H_ */
