/*
 * tile.h
 *
 *  Created on: 27.09.2011
 *      Author: demmeln
 */

#ifndef TILE_H_
#define TILE_H_


#include "definitions.h"


namespace mnp {

struct Tile {

	enum Type {
		TileWall		= 1 << 1,
		TileGoal		= 1 << 2,
	};

	Tile *neighbours[4];

	Board *board; // TODO: needed?
	index index; // own index in tile array. TODO: needed ?

	int box; // -1 or index in box-array
	uint8_t type;

	inline bool isBox() {
		return box >= 0;
	}

	inline bool isWall() {
		return type & TileWall;
	}

	inline bool isGoal() {
		return type & TileGoal;
	}

	inline bool isFree() {
		return !isBox() && !isWall();
	}
};



};

#endif /* TILE_H_ */
