/*
 * move.cc
 *
 *  Created on: 11.10.2011
 *      Author: demmeln
 */

#include "move.h"
#include "board.h"

namespace mnp {


Move::Move(Pos box_pos, Dir dir, const Board *board):
	mBoxIndex(board->tileIndex(box_pos)),
	mDir(dir)
{
}

string Move::toString(const Board *board) const {
	stringstream ss;
	ss << "Move box ";
	if (board)
		ss << board->indexToPos(mBoxIndex).toString();
	else
		ss << "index " << mBoxIndex;
	ss << " " << cDirNames[mDir];
	return ss.str();
}

index_t Move::getNextIndex(const Board *board) const {
	return board->tileIndex(mBoxIndex, mDir, 1);
}

index_t Move::getPlayerIndex(const Board *board) const {
	return board->tileIndex(mBoxIndex, mDir, -1);
}


}

