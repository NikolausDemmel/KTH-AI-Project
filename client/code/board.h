/*
 * board.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "tile.h"
#include "tilegraphnode.h"
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
	////////////////////////////////////////////////////////////////////////////////
	// CONSTRUCTION
	////////////////////////////////////////////////////////////////////////////////
	Board(){};
	Board(string board);
	Board(const char* board);
	Board(const char* fileName, unsigned int boardNumber);
	~Board();

	// FIXME: copy constructor and assignment operator!!


	////////////////////////////////////////////////////////////////////////////////
	// SEARCH
	////////////////////////////////////////////////////////////////////////////////

public:
	bool isSolved();
	void restoreInitialPlayerIndex();
	void setPlayerIndex(index_t player);



	// change the board according to the move
	void applyMove(const Move &move, SearchType type);

	// undo the move, change the board again
	void undoMove(const Move &move, SearchType type);

	// calls visitTile
	void generateMoves(vector<Move> &moves, SearchType type);
	
	inline int minMovesReqd() const {
		int moves=0;
		foreach (index_t box_index, mBoxes ) {
			moves+=mTiles[box_index].distanceClosestGoal;
		}
		return moves;
	}


private: // FIXME
	// visit the neighboring tiles and set the visitflag
	// if there is a neighboring box, add the tilepos + direction of the box to the vector of moves
	void visitTile(index_t tileIndex, vector<Move> &moves);

	void reverseVisitTile(index_t tile, vector<Move> &moves);

	uint countMissingGoals() const;

	// FIXME: which flags do we need
	// reset the visit- and extra-flags
	void clearFlags();


	////////////////////////////////////////////////////////////////////////////////
	// GAME SIMULATION
	////////////////////////////////////////////////////////////////////////////////
public: // FIXME
	void simulateActions(const char* actions);
	bool actionsForMove(string &actions, const Move &move) const;

private: // FIXME
	bool doAction(Dir toWhere);
	void undoAction(Dir fromWhere, bool unPush);
	bool shortestPathSearch(string &actions, index_t start, index_t end) const;

	////////////////////////////////////////////////////////////////////////////////
	// INDEX CALCULATIONS
	////////////////////////////////////////////////////////////////////////////////

private:

	static uint calculateIndexBits(uint size);

public:

	inline bool isValidIndex(index_t index) const {
		return index >= 0 && index < mSize;
	}

	inline coord_t indexToX(index_t index) const {
		return index % mWidth;
	}

	inline coord_t indexToY(index_t index) const {
		return index / mWidth;
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


	// FIXME: do we need those??

	uint size() const {
		return mSize;
	}

	index_t getPlayerIndex() const {
		return mPlayerIndex;
	}

	// FIXME: do we need those??
//	uint8_t getTile(int x, int y) const {
//		return mBoard[TileIndex(x,y)];
//	}
//
//	uint8_t getTile(int index) const {
//		return mBoard[index];
//	}



private:

//	void SetTile(int x, int y, uint8_t value) {
//		mBoard[TileIndex(x, y)] = value;
//	}

	////////////////////////////////////////////////////////////////////////////////
	// PARSING AND PRINTING
	////////////////////////////////////////////////////////////////////////////////
public:

	void printBoard(uint8_t printFlags = 0) const;

	string boardToString(uint8_t printFlags = 0, const vector<TileGraphNode> * const nodes = 0, bool print_dead=false, bool print_dist=false) const;
private:

	void parseBoard(const char* board);

	void parseBoardFromFile(const char *fileName, size_t boardNumber);

	// tile from character ignoring the player and the boxes
	static Tile parseTile(char c);

	// character from tile ignoring the player, used e.g. for printing the board
	static char tileCharacter(const Tile &t, const TileGraphNode * const node, bool print_dead, bool print_dist);

	// FlagString returns the string-code for the color of the tile
	// set colour if tile is visited, ...
	static const char* flagString(const Tile &tile, uint8_t flags);

	// end of the color-code
	static const char* endFlagString(uint8_t flags);

	////////////////////////////////////////////////////////////////////////////////
	// HASH-VALUE
	////////////////////////////////////////////////////////////////////////////////

public:

	uint64_t getHash() const {
		return mHashValue;
	}

	// computes the hash value from scratch
	uint64_t computeHashValue() const ;

	void recomputeHashValue() {
		mHashValue = computeHashValue();
	}

	void getAllPlayerPosHashForward(vector<index_t> &possiblePlayerInd, vector<pair<uint64_t,uint64_t>> &hashes, bool hash);
	void getAllPlayerPosHashBack(vector<index_t> &possiblePlayerInd, vector<pair<uint64_t,uint64_t>> &hashes, bool hash);


	// computes an alternative hash value from scratch
	uint64_t computeHash2Value();

	// xor the number to the hash value
	void updateHash(uint64_t zobristNumber);



private:

	// create the array of zobrist numbers with random numbers;
	void initializeZobrist();

	// get the ZobristNumber of the tile at a certain index.
	uint64_t getZobristBox(index_t tile) const;
	uint64_t getZobristPlayer(index_t tile) const;






	////////////////////////////////////////////////////////////////////////////////
	// GROUPINFO
	////////////////////////////////////////////////////////////////////////////////

public:

	bool isReachable(index_t tile) {
		return sameGroup(mPlayerIndex, tile);
	}

	bool sameGroup(index_t a, index_t b) {
		return false;
		// TODO
	}

	void joinGroup(index_t a, index_t b) {
		// set parent of representative of b to representative of a
		// TODO
	}

	void splitGroup(index_t a, index_t b) {
		// TODO
	}


	////////////////////////////////////////////////////////////////////////////////
	// DATA
	////////////////////////////////////////////////////////////////////////////////

public: // FIXME
	// TODO: store list of possible moves here or in agent class ??


	// COPYING VS UPDATING INFORMATION
	//
	// Since we do depth first search we don't need to copy the board, but
	// rather do and undo moves. Thus we get no performance penalty from
	// storing auxiliary information in the board and we don't need to focus
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

	uint mSize; // mWidth * mHeight
	uint mWidth;
	uint mHeight;
	uint mIndexBits; // How many bits do we need to store an
                       // index. esssentially ceil(log2(size)).
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
	vector<index_t> mGoals;

	// This is an array of indices, to be potentially copied instead of
	// modified. FIXME: actually maybe never copy; inverstigate...
	index_t *mGroupInfo; // TODO: better name? TODO: use uint8_t ? TODO:
                         // evaluate: save group size ?
	// FIXME: destructor

	// How many goals are lacking a box. If this reaches 0 we have solved the
	// board.
	uint mMissingGoals;

	// The current hash value and the lists of random numbers for generating /
	// updated this value.
	uint64_t mHashValue;
	vector<uint64_t> mZobristBoxes; // array of size mSize
	vector<uint64_t> mZobristPlayer; // array of size mSize
};


};

#endif /* BOARD_H_ */
