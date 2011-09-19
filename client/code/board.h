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
		delete mBoard;
	}

	void PrintBoard() {
		cout << BoardToString() << endl;
	}

	string ToString() {
		// TODO
	}

	// TODO: to-string operator

	void ApplyMove(const Move &move); // TODO

	void UndoMove(const Move &move); // TODO

	// TODO: function for generating all possible moves
	// (breadth/depth first search starting from player tile, with flags for visited tiles).

	// TODO: function that takes a move (rather, a position), and generates the steps to execute this move
	// _breadth_ first search (to get the shortest possible way to get there).

private:
	void ParseBoard(const char* board) {
		int width = 0;
		int height = 0;

		int currWidth = 0;
		for(int i = 0; i < strlen(board); ++i) {
			if(board[i] == '\n') {
				++height;
				width = max(width, currWidth);
				currWidth = 0;
			}
			else {
				++currWidth;
			}
		}

		mBoard = new uint8_t[width*height];

		int y = height - 1;
		int x = 0;
		for(int i = 0; i < strlen(board); ++i) {
			if(board[i] == '\n') {
				// FIXME: maybe find all unreachable regions and make them "invalid" straight away.
				// for now we just fill trailing tiles with walls.
				for(; x < width; ++x) {
					mBoard[x][y] = TileWall; // FIXME
				}
				x = 0;
				--y;
			} else {
				mBoard[x][y] = ParseTile(board[i]); // FIXME
				if (board[i] == '@' || board[i] == '+') {
					mPlayerPos = Pos(x,y);
				}
				++x;
			}
		}
	}

	inline int TileIndex(int x, int y) {
		return mWidth * y + x;
	}

	uint8_t GetTile(int x, int y) {
		return mBoard[TileIndex(x,y)];
	}

	void SetTile(int x, int y, uint8_t value) {
		mBoard[TileIndex(x, y)] = value;
	}

	static uint8_t ParseTile(char c) {
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

	static char TileCharacter(uint8_t t) {
		switch(t) {
		case TileEmpty:
			return ' ';
		case TileWall:
			return '#';
			// TODO: finish
		}
	}

private:

	int mWidth;
	int mHeight;
	Pos mPlayerPos;
	uint8_t *mBoard;
};

};

#endif /* BOARD_H_ */
