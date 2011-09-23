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
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>




using namespace std;

namespace mnp {

class Board
{
public:
	Board(const char* board) {
		ParseBoard(board);
	}

	Board(string board) {
		ParseBoard(board.c_str());
	}

	Board(const char* fileName, unsigned int boardNumber);

	~Board() {
	}

	// used for testing
	void PrintBoard(uint8_t printFlags = 0) const {
		cout << BoardToString(printFlags) << endl;
	}

	// convert to board to a string
	string BoardToString(uint8_t printFlags = 0, vector<TileNode> *nodes = 0) const;

	// change the board according to the move
	void ApplyMove(const Move &move);

	// undo the move, change the board again
	void UndoMove(const Move &move);

	// calls VisitTile
	// debugging
	void GenerateMoves(vector<Move> &moves);

	bool isSolved();

	bool doAction(Dir toWhere);

	void undoAction(Dir fromWhere,bool unPush);

	void simulateActions(const char* actions);

	/*Pos getPlayerPos(){
		return mPlayerPos;
	}

	vector<int> getBoxes(){
		return mBoxes;
	}

	uint8_t* getBoard(){
		return mBoard;
	}*/

	void restoreInitialPlayerPos() {
		setPlayerPos(mInitialPlayerPos);
	}

private:
	// visit the neighboring tiles and set the visitflag
	// if there is a neighboring box, add the tilepos + direction of the box to the vector of moves
	void VisitTile(int tileIndex, vector<Move> &moves);

	// reset the visit- and extra-flags
	void ClearFlags();

	int countMissingGoals() const;

	void ParseBoard(const char* board);

	inline bool IsIndex(int index) const {
		return index >= 0 && index < mWidth*mHeight;
	}

	static int calculateIndexBits(int size);

public:
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

	int size() const {
		return mWidth*mHeight;
	}

	Pos getPlayerPos() const {
		return mPlayerPos;
	}

	uint8_t getTile(int x, int y) const {
		return mBoard[TileIndex(x,y)];
	}

	uint8_t getTile(int index) const {
		return mBoard[index];
	}

	uint64_t getHash() const {
		return mHashValue;
	}

	uint64_t getHash2() {
		return computeHash2Value();
	}

private:

	inline Pos TileIndexToPos(int index) {
		return Pos(index%mWidth, index/mWidth);
	}

	inline void setPlayerPos(Pos pos) {
		mPlayerPos = pos;
		mPlayerIndex = TileIndex(pos);
	}

	void SetTile(int x, int y, uint8_t value) {
		mBoard[TileIndex(x, y)] = value;
	}

	// returns the type of the tile
	static uint8_t ParseTile(char c);


	// returns the character of the tile, used e.g. for printing the board
	static char TileCharacter(uint8_t t, TileNode *node);

	// FlagString returns the string-code for the color of the tile
	// set colour if tile is visited, ...
	static const char* FlagString(uint8_t tile, uint8_t flags);

	// end of the color-code
	static const char* EndFlagString(uint8_t flags);



	// create the array of zobrist numbers with random numbers;
	void initializeZobrist();

	// get the ZobristNumber of the tile at a certain index.
	uint64_t getZobristBox(int tileIndex) const;
	uint64_t getZobristPlayer(int tileIndex) const;

	// xor the number to the hash value
	void updateHash(uint64_t zobristNumber);

	// computes the hash value from scratch
	uint64_t computeHashValue() const ;

	// computes an alternative hash value from scratch
	uint64_t computeHash2Value();

private:
	// TODO: potential minor optimization: store mSize to avoid calculation mWidth*mHeight.

	int mIndexBits;

	int mWidth;
	int mHeight;
	Pos mPlayerPos;
	int mPlayerIndex;
	Pos mInitialPlayerPos;
	uint8_t *mBoard; // use vector instead
	vector<int> mBoxes; // save the index of the boxes on the boards

	int mMissingGoals;

	uint64_t mHashValue;
	vector<uint64_t> mZobristBoxes;
	vector<uint64_t> mZobristPlayer;
};

};

#endif /* BOARD_H_ */
