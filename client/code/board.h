/*
 * board.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "common.h"
#include "move.h"
#include <string>
#include <cmath>
#include <iostream>
#include <cstring>
#include <vector>




using namespace std;

namespace mnp {

// TODO: Once the board is done, we need some kind of Player or Solver class, that does the
// search with iterative deepening or breadth first or something until it finds a solution
// and the returns the solution string to the client who sends it to the server.


// TODO: put implementation in .cc file.

class Board
{
public:
	Board(const char* board) {
		ParseBoard(board);
	}

	Board(string board) {
		ParseBoard(board.c_str());
	}

	~Board() {
	}

	void PrintBoard(uint8_t printFlags = 0) const {
		cout << BoardToString(printFlags) << endl;
	}

	string BoardToString(uint8_t printFlags) const;

	// TODO: to string operator?

	void ApplyMove(const Move &move); // TODO

	void UndoMove(const Move &move); // TODO

	void GenerateMoves(vector<Move> &moves);

private:
	void VisitTile(int tileIndex, vector<Move> &moves);
	void ClearFlags();

	void ParseBoard(const char* board);

	inline bool IsIndex(int index) const {
		return index >= 0 && index < mWidth*mHeight;
	}

	inline int TileIndex(int x, int y) const {
		assert(IsIndex(mWidth * y + x));
		return mWidth * y + x;
	}

	inline int TileIndex(const Pos &pos) const {
		return TileIndex(pos.x, pos.y);
	}

	inline int TileIndex(int index, Dir offset) const {
		switch(offset) {
		case Up:
			assert(IsIndex(index + mWidth));
			return index + mWidth;
		case Left:
			assert(IsIndex(index - 1));
			return index - 1;
		case Down:
			assert(IsIndex(index - mWidth));
			return index - mWidth;
		case Right:
			assert(IsIndex(index + 1));
			return index + 1;
		}
	}

	inline Pos TileIndexToPos(int index) {
		return Pos(index%mWidth, index/mWidth);
	}

	uint8_t GetTile(int x, int y) const {
		return mBoard[TileIndex(x,y)];
	}

	void SetTile(int x, int y, uint8_t value) {
		mBoard[TileIndex(x, y)] = value;
	}

	static uint8_t ParseTile(char c);

	static char TileCharacter(uint8_t t);

	static const char* FlagString(uint8_t tile, uint8_t flags);
	static const char* EndFlagString(uint8_t flags);



private:
	// TODO: potential minor optimization: stor mSize to avoid calculation mWidth*mHeight.
	int mWidth;
	int mHeight;
	Pos mPlayerPos;
	uint8_t *mBoard;
};

};

#endif /* BOARD_H_ */
