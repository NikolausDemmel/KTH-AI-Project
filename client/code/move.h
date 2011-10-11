/*
 * move.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "definitions.h"

namespace mnp {

class Board;

class Move
{
public:
	Move(index_t box_index, Dir dir):
		mDir(dir),
		mBoxIndex(box_index)
	{
	}

	Move(Pos pos, Dir dir, const Board *board):
		mBoxIndex(board->tileIndex(pos)),
		mDir(dir)
	{
	}

	string toString(const Board *board = 0) const;

	Pos getBoxIndex() const{
		return mBoxIndex;
	}
	Dir getMoveDir() const {
		return mDir;
	}

	index_t getPlayerIndex(const Board *board = 0) const;

private:
	index_t mBoxIndex;
	Dir mDir;
};

};


#endif /* MOVE_H_ */
