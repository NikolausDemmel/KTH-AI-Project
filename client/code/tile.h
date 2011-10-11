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

	enum StaticType {
		TileWall		= 1 << 1,
		TileGoal		= 1 << 2,
	};


	int box; // -1 or index in box-array
	uint8_t static_type; // Logic combination of StaticType flags. Not all combinations are valid.

	inline bool isBox() {
		return box >= 0;
	}

	inline bool isWall() {
		return static_type & TileWall;
	}

	inline bool isGoal() {
		return static_type & TileGoal;
	}

	inline bool isFree() {
		return !isBox() && !isWall();
	}

	// The original idea of storing pointers to neighbours and the board and
	// the own index of the tile was that we can go from a tile object to its
	// neighbors and get their index and also look up stuff in the
	// board. However since this tile class is should mostly hold data and not
	// implement any algorithms (they will be implemented in the board class),
	// this is maybe simply useless. E.g. the index of a neighboring board can
	// be computed much quicker directly, e.g. left is index-1, right is
	// index+1, up is index+boardwidth and down is index-boardwidth.
    // FIXME: needed?
//	Tile *neighbours[4]; // the neighboring cells, or 0 for the non existing
//                         // neighbors of border cells
//	Board *board;
//	index_t index; // own index in tile array.

};



};

#endif /* TILE_H_ */
