/*
 * board.cc
 *
 *  Created on: 20.09.2011
 *      Author: astridrupp
 */


#include "board.h"

namespace mnp {




char* Board::BoardToString() {
	char* board;
	for(int y = 0; y<mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			if (((mPlayerPos.x == x) && (mPlayerPos.y == mHeight-1-y))
					&& (mBoard[TileIndex(x, mHeight-1-y)] == TileGoal))
				board[x+y*(mWidth+1)] = '+';
			else if ((mPlayerPos.x == x) && (mPlayerPos.y == mHeight-1-y)
					&& (mBoard[TileIndex(x, mHeight-1-y)] == TileEmpty))
				board[x+y*(mWidth+1)] = '@';
			else
				board[x + y*(mWidth+1)] = TileCharacter(mBoard[TileIndex(x, mHeight-1-y)]);
		}
		board[(mWidth+1)*(y+1)] = '\n';

	}

	return board;
}

void Board::ApplyMove(const Move &move) {
} // TODO

void Board::UndoMove(const Move &move) {
} // TODO

void Board::GenerateMoves() {
} // TODO


// TODO: function for generating all possible moves
// (breadth/depth first search starting from player tile, with flags for visited tiles).

// TODO: function that takes a move (rather, a position), and generates the steps to execute this move
// _breadth_ first search (to get the shortest possible way to get there).

void Board::ParseBoard(const char* board) {
	mWidth = 0;
	mHeight = 0;

	int currWidth = 0;
	for (int i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			++mHeight;
			mWidth = max(mWidth, currWidth);
			currWidth = 0;
		} else {
			++currWidth;
		}
	}

	mBoard = new uint8_t[mWidth*mHeight];

	int y = mHeight - 1;
	int x = 0;
	for (int i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			// FIXME: maybe find all unreachable regions and make them "invalid" straight away.
			// for now we just fill trailing tiles with walls.
			for (; x < mWidth; ++x) {
				mBoard[TileIndex(x, y)] = TileWall;
			}
			x = 0;
			--y;
		} else {
			mBoard[TileIndex(x, y)] = ParseTile(board[i]);
			if (board[i] == '@' || board[i] == '+') {
				mPlayerPos = Pos(x, y);
			}
			++x;
		}
	}
}

	uint8_t Board::ParseTile(char c) {
		switch(c) {
		case ' ':
			return TileEmpty;
		case '#':
			return TileWall;
		case '$':
			return TileBox;
		case '.':
			return TileGoal;
		case '+':
			return TileGoal;
		case '*':
			return TileGoal | TileBox;
		case '@':
			return TileEmpty;
		default:
			throw "Invalid tile character.";
		}
	}

	char Board::TileCharacter(uint8_t t) {  //FIXME undo flag
		switch(t) {
		case TileEmpty:
			return ' ';
		case TileWall:
			return '#';
		case TileBox:
			return '$';
		case TileGoal:
			return '.';
		case TileGoal | TileBox:
			return '*';
		default:
			throw "Invalid tile type.";
		}
	}




};

