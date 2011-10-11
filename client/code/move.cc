/*
 * move.cc
 *
 *  Created on: 11.10.2011
 *      Author: demmeln
 */

#include "move.h"
#include "board.h"

namespace mnp {


string Move::toString(const Board *board) const {
	stringstream ss;
	ss << "Move box " << (board ? board->indexToPos(mBoxIndex).toString() : mBoxIndex) << " " << dirToString(mDir);
	return ss.str();
}

index_t Move::getPlayerIndex(const Board *board = 0) const {
	return board->tileIndex(mBoxIndex, mDir, -1);
}


}

