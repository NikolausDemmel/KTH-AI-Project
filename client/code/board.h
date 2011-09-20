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

	~Board() {
	}

	void PrintBoard() {
		cout << BoardToString() << endl;
	}

	char* BoardToString();

	void ApplyMove(const Move &move); // TODO

	void UndoMove(const Move &move); // TODO

	void GenerateMoves(); // TODO



private:
	void ParseBoard(const char* board);

	inline int TileIndex(int x, int y) {
		return mWidth * y + x;
	}

	uint8_t GetTile(int x, int y) {
		return mBoard[TileIndex(x,y)];
	}

	void SetTile(int x, int y, uint8_t value) {
		mBoard[TileIndex(x, y)] = value;
	}

	static uint8_t ParseTile(char c);

	static char TileCharacter(uint8_t t);


private:



	int mWidth;
	int mHeight;
	Pos mPlayerPos;
	uint8_t *mBoard;
};

};

#endif /* BOARD_H_ */
