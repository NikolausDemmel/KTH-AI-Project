/*
 * board.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "tile.h"
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

// TODO: evaluate if it makes sense to have an array of indices of the non-wall tiles
// TODO: IDEA: store a list of indeces, that are actually relevat, i.e. not walls and not never reachable tiles!!!




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
	void printBoard(uint8_t printFlags = 0) const {
		cout << boardToString(printFlags) << endl;
	}

	// convert to board to a string
	string boardToString(uint8_t printFlags = 0, vector<TileNode> *nodes = 0) const;

	// change the board according to the move
	void applyMove(const Move &move);

	// undo the move, change the board again
	void undoMove(const Move &move);

	// calls VisitTile
	// debugging
	void generateMoves(vector<Move> &moves);

	bool isSolved();

	bool doAction(Dir toWhere);

	void undoAction(Dir fromWhere,bool unPush);

	void simulateActions(const char* actions);

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

	static int calculateIndexBits(int size);

public:

	inline bool isValidIndex(index_t index) const {
		return index >= 0 && index < mSize;
	}

	inline coord_t indexToX(index_t index) const {
		return index % mWidth;
	}

	inline coord_t indexToY(index_t index) const {
		return index % mWidth;
	}

	inline Pos indexToPos(index_t index) const {
		return Pos(index, this);
	}

	inline index_t tileIndex(coord_t x, coord_t y) const {
		assert(isValidIndex(mWidth * y + x));
		return mWidth * y + x;
	}

	inline index_t tileIndex(Pos pos) const {
		return tileIndex(pos.x, pos.y);
	}

	// pass -1 as invert to get the inverse direction, pass 2 to offset 2 tiles in the given direction etc...
	inline index_t tileIndex(index_t index, Dir offset, int factor = 1) const {
		switch(offset) {
		case Up:
			assert(isValidIndex(index + mWidth * factor));
			return index + mWidth * factor;
		case Left:
			assert(isValidIndex(index - 1 * factor));
			return index - 1 * factor;
		case Down:
			assert(isValidIndex(index - mWidth * factor));
			return index - mWidth * factor;
		case Right:
			assert(isValidIndex(index + 1 * factor));
			return index + factor;
		}
	}

//	size_t size() const {
//		return mSize;
//	}
//
//	Pos getPlayerPos() const {
//		return mPlayerPos;
//	}
//
//	uint8_t getTile(int x, int y) const {
//		return mBoard[TileIndex(x,y)];
//	}
//
//	uint8_t getTile(int index) const {
//		return mBoard[index];
//	}

	uint64_t getHash() const {
		return mHashValue;
	}

//	uint64_t getHash2() {
//		return computeHash2Value();
//	}

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


	/**
	 * HASH
	 */

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

	/**
	 * GROUPINFO
	 */

	bool isReachable(index_t tile) {
		return sameGroup(mPlayerIndex, tile);
	}

	bool sameGroup(index_t a, index b) {
		// TODO
	}

	void joinGroup(index a, index b) {
		// set parent of representative of b to representative of a
		// TODO
	}

	void splitGroup(index a, index b) {
		// TODO
	}



private:
	// TODO: store list of possible moves here or in agent class ??


	// COPYING VS UPDATING INFORMATION
	//
	// Since we do depth first search we don't need to copy the board, but
	// rather do and undo moves. Thus we get no performance penalty from
	// storing auxiliary information in the borad and we don't need to focus
	// on minimizing the size of the board data. With this in mind, we try to
	// compute much supplimentry information about the current board state,
	// that is needed in every step of the search, incrementally rather than
	// all the time from scratch. Examples are the hash value, the sets of
	// connected tiles.
	//
	// Many changes are quick to undo, for example making a move and undoing
	// it is roughly equivalent. Other changes however are not symmetric for
	// doing and undoing. For these kinds of changes we condsider saving part
	// of the board state during search und undoing them buy simply copying
	// the stored value. FIXME: I just thought of a reason why we don't need
	// to do this for the sets of connected tiles... Since that was the only
	// example so far, maybe just scrap the whole idea. (Its still interesting
	// from a theoretical point of view)

	index_t mSize; // mWidth * mHeight
	size_t mWidth;
	size_t mHeight;
	size_t mIndexBits; // How many bits do we need to store an
                       // index. esssentially ceil(log2(size)).
	size_t mNumberOfBoxes; // How many boxes are on the board. Obviously
                           // invariant over one particular game.
	index_t mInitialPlayerIndex; // What was the initial player position. We
                                 // need this to be able to undo the first
                                 // move.

	index_t mPlayerIndex; // What tile is the player currently on.

	// We use an array of tiles to represent the board. The size is mSize.
	// Position 0,0 corresponds to the bottom left cornern. Position mWidth,
	// mHeight to the top right corner.  Also, position x,y on the board
	// corresponds to the array index y*mWidth+x.  However internally we use
	// only indices directly and avoid going back and forth between index and
	// coordinate representation of a position.
	vector<Tile> mTiles;

	// Array of all boxes, so that we can avoid going through the whole board
	// when we want to do some operation on all boxes.
	vector<index_t> mBoxes;

	// This is an array if indices, to be potentially copied instead of
	// modified. FIXME: actually maybe never copy; inverstigate...
	index_t *mGroupInfo; // TODO: better name? TODO: use uint8_t ? TODO:
                         // evaluate: save group size ?

	// How many goals are lacking a box. If this reaches 0 we have solved the
	// board.
	size_t mMissingGoals;

	// The current hash value and the lists of random numbers for generating /
	// updated this value.
	uint64_t mHashValue;
	vector<uint64_t> mZobristBoxes; // array of size mSize
	vector<uint64_t> mZobristPlayer; // array of size mSize
};


};

#endif /* BOARD_H_ */
