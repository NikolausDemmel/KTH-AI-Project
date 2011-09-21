/*
 * board.cc
 *
 *  Created on: 20.09.2011
 *      Author: astridrupp
 */


#include "board.h"

namespace mnp {



// TODO: rename to ToString(..) ?
string Board::BoardToString(uint8_t printFlags = 0) const
{
	stringstream board;
	for(int y = mHeight-1; y >= 0; --y) {
		for (int x = 0; x < mWidth; x++) {
			int index = TileIndex(x, y);
			board << FlagString(mBoard[index], printFlags);
			if ((mPlayerPos.x == x) && (mPlayerPos.y == y))
				if (mBoard[index] & TileGoal)
					board << '+';
				else
					board << '@';
			else
				board << TileCharacter(mBoard[index]);
			board << EndFlagString(printFlags);
		}
		board << '\n';
	}

	return board.str();
}

void Board::ApplyMove(const Move &move) {
} // TODO

void Board::UndoMove(const Move &move) {
} // TODO


// FIXME: would it not be quicker to incrementally update all the reachability information
// also maybe store the boxes as a list of positions
void Board::GenerateMoves(vector<Move> &moves)
{
	VisitTile(TileIndex(mPlayerPos), moves);
#ifdef DEBUG
	cout << BoardToString(TileFlagMask);
	cout << "Possible moves:" << endl;
	for (vector<Move>::iterator iter = moves.begin(); iter != moves.end(); ++iter) {
		cout << iter->ToString() << endl;
	}
#endif
	ClearFlags();
}

void Board::VisitTile(int tileIndex, vector<Move> &moves)
{
	if ( (mBoard[tileIndex] & TileVisitedFlag) == 0 ) {
		mBoard[tileIndex] |= TileVisitedFlag;
		for (int dir = 0; dir < 4; ++dir) {
			int next = TileIndex(tileIndex, static_cast<Dir>(dir));
			if ((mBoard[next] & TileBox) && (TileFree(mBoard[TileIndex(next,static_cast<Dir>(dir))]))) {
				mBoard[tileIndex] |= TileExtraFlag;
				moves.push_back(Move(TileIndexToPos(next), static_cast<Dir>(dir)));
			} else if (TileFree(mBoard[next])) {
				VisitTile(next, moves);
			}
		}
	}
}

void Board::ClearFlags() {
	for (int i = 0; i < mWidth*mHeight; ++i) {
		mBoard[i] &= ~TileFlagMask;
	}
}

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

	char Board::TileCharacter(uint8_t t) {
		switch(t & (~TileFlagMask)) {
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

const char* Board::FlagString(uint8_t tile, uint8_t flags) {
	if (flags & tile & TileVisitedFlag)
		if (flags & tile & TileExtraFlag)
			return "\e[43m";
		else
			return "\e[42m";
	else
		return "";
}

const char* Board::EndFlagString(uint8_t flags) {
	if (flags)
		return "\e[0m";
	else
		return "";
}



};

