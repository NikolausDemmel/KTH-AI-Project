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
		mHashTable(26) // 2^26 entries if 64 bit values should be 512 MB
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

	void findSolution();

	SearchResult depthLimitedSearch(uint depth);

	string executeSolution();

	// TODO: find another class where we can put this (maybe Board)
	static bool shortestPathSearch(string &actions, const Board &board, index_t start, index_t end);
	static bool actionsForMove(string &actions, const Board &board, const Move &move);

private:

	Board *mBoard;

	HashTable mHashTable;

public: // FIXME: should be private

	list<Move> solutionMoves;
};

};

#endif /* AGENT_H_ */
