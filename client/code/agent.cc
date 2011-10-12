/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include <sstream>
#include "agent.h"

namespace mnp {

void Agent::findSolution()
{
	SearchResult result;
	uint depth = 1;

	mSolutionMoves.clear();

	do {
#ifdef INFO
		cout << "[agent] trying depth " << depth << endl;
		cout << "[agent] # of boxes " << mBoard->mBoxes.size() << endl;
		cout << "[agent] log size of board " << mBoard->mIndexBits << endl;
#endif
		result = depthLimitedSearch(depth++);
#ifdef INFO
		cout << "[agent] search result for this depth: " << cSearchResultNames[result] << endl;
		mHashTable.printStatistics();
#endif
		// FIXME:
		// mHashTable.clear();
		mHashTable.clearStatistic();
	} while (result == CutOff);

	mBoard->restoreInitialPlayerIndex();

#ifdef INFO
	cout << "[agent] result of search was " << cSearchResultNames[result] << endl;
#endif

	if (result==Failure)
		throw "[agent] board can't be solved."; // Fail more grance-fully
	else
		return;

}

SearchResult Agent::depthLimitedSearch(uint depth)
{

	// 1. Base Cases
	if (depth == 0) return CutOff;
	if (mBoard->isSolved()) return Solution;

	// 2. Hashtable lookup
	if (mHashTable.lookup(mBoard->getHash(), depth))
		return CutOff; // FIXME: do we need to store CutOff vs Failure in the hash table?

	// 3. Deadlock detection
	// TODO: Deadlock detection should kick in here:


	// 4. Search one step deeper
	bool cutoff_occured = false;
	vector<Move> possible_moves;
	SearchResult result;

	mBoard->generateMoves(possible_moves);

	foreach ( const Move &move, possible_moves) {

		mBoard->applyMove(move);
//		myBoard->PrintBoard();
		result = depthLimitedSearch(depth-1);
		mBoard->undoMove(move);

		if (result==CutOff) {
			cutoff_occured = true;
		} else if (result != Failure) {
			mSolutionMoves.push_front(move);
			return Solution;
		}
	}

	// 5. assess outcome
	if (cutoff_occured) return CutOff;
	else return Failure;

}

void Agent::setBoard(Board *board) {
	mBoard = board;
}

string Agent::executeSolution() {

	// FIXME: should we really do this here
	// FIXME: ^ we can do it after each method is completed. (for instance, in the function body of find solutions)
	//myBoard->restoreInitialPlayerPos();

#ifdef INFO
	cout << "Execution Solution." << endl;
#endif

	stringstream solution;
	auto move_iter = mSolutionMoves.begin();

	for(; move_iter != mSolutionMoves.end(); ++move_iter) {
		const Move &move = *move_iter;

		string actions;
		if (! (mBoard->actionsForMove(actions, move))) {
			throw "Invalid move sequence.";
		}
		solution << actions;

#ifdef INFO
		solution << " "; // use this to tell moves apart
#endif

		mBoard->applyMove(move);

#ifdef INFO
		cout << "Applying move " << move.toString(mBoard) << endl;
		mBoard->printBoard();
#endif

	}

	// FIXME: big questions: I think we need to maintain an invariant that after anything agent does to a board, the board remains the same?
	// I(yg) made a function simulate actions that takes in the string of solution and runs them on a board *BUT* also undoes them.
	//
	while (move_iter != mSolutionMoves.begin()) {
		--move_iter;
#ifdef INFO
		cout << "Undoing move " << move_iter->toString(mBoard) << endl;
#endif
		mBoard->undoMove(*move_iter);
	}
	mBoard->restoreInitialPlayerIndex();

	return solution.str();
}

}; // namespace



