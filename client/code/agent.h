/*
 * agent.h
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#ifndef AGENT_H_
#define AGENT_H_

#include "common.h"
#include "board.h"
#include "hash_table.h"
#include <list>

using namespace std;

namespace mnp {

class Agent{
public:

	Agent():
		mHashTable(26), // 2^26 entries if 64 bit values should be 512 MB
		mBackwardHashTable(26)
	{
	}

	//~Agent(); // FIXME

	/*	void search(Board board);
	 *
	 *	bool dfs(Board board, int depth, vector<Move> &solutionMoves);
	 *
	 *	// when we know how to do it, we should somehow create the paths between the moves
	 *	string findPath();
	 *
	 *	// when we know everything, we have to write the solution into a string and send it
	 *	string submitSolution();
	 */

	void setBoard(Board *board);

	void setBackBoard(Board *board);

	void findSolution();
	
	void setDistanceMetric();
	
	void findDeadTiles();

	SearchResult depthLimitedSearch(uint depth, Board *board, SearchType type, uint64_t &hashMeeting, bool deadlockSearch=false);

	string executeSolution();

private:

	Board *mBoard;
	Board mBackBoard;

	HashTable mHashTable;
	HashTable mBackwardHashTable;

public: // FIXME: should be private

	list<Move> mSolutionMoves;
};

};

#endif /* AGENT_H_ */
