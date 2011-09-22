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
//#include <list>
#include <stack>

namespace mnp {

enum SearchResult {
	Solution,
	Failure,
	CutOff
};

class Agent{
public:
	// TODO: this does the
	// search with iterative deepening or breadth first or something until it finds a solution
	// and the returns the solution string to the client who sends it to the server.

	//Agent();

	//~Agent();

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

	void setBoard(Board *aBoard);
	//{myBoard = aBoard;}

	void findSolution();

	SearchResult depthLimitedSearch(int depth);

	//breadth first search for finding the shortest path to a certain pos
	vector<int> pathSearch(Board board, Pos start, Pos end);

private:

	Board *myBoard;
public:
	stack<Move> solutionMoves;

/*	list<Move> SolutionMoves;
	list<Move>::iterator it;
 */ // Use this for list


}; // class

}; // namespace


#endif /* AGENT_H_ */
