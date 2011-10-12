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

	static const int NoBox = -1;

	enum StaticType {
		Empty 		= 0,
		Wall		= 1 << 0,
		Goal		= 1 << 1
	};

	enum Flags {
		VisitedFlag		= 1 << 0,
		ExtraFlag		= 1 << 1
	};



	uint8_t static_type; // Logic combination of StaticType flags. Not all combinations are valid.
	int box; // -1 or index in box-array
	uint8_t flags;



	explicit Tile(uint8_t static_type_ = Empty, int box_ = NoBox, uint8_t flags_ = 0):
		static_type(static_type_),
		box(box_),
		flags(flags_)
	{
	}

	inline bool flagsSet(uint8_t mask) const {
		return (flags & mask) == mask;
	}

	inline void setFlags(uint8_t mask) {
		flags |= mask;
	}

	inline void unsetFlags(uint8_t mask) {
		flags &= ~mask;
	}

	inline void clearFlags() {
		flags = 0;
	}

	inline bool isBox() const {
		return box >= 0;
	}

	inline bool isWall() const {
		return static_type & Wall;
	}

	inline bool isGoal() const {
		return static_type & Goal;
	}

	inline bool isFree() const {
		return !isBox() && !isWall();
	}

	inline int removeBox() {
		int tmp = box;
		box = NoBox;
		return tmp;
	}

	inline void setBox(int box_) {
		box = box_;
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
