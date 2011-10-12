/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include <sstream>
#include "agent.h"

namespace mnp {

/*
	void Agent::search(Board board){
		int val = dfs(board, 2);

void Agent::search(Board board){
	vector<Move> solutionMoves;
	bool val = false;
	int depth = 1;
	while(!val){
	val = dfs(board, depth, solutionMoves); // as long as val is false, increase the depth
	depth++;
	}
	// now we have to call findPath
}

string Agent::findPath(Board board){

	//1st step: get position of the player.
	Pos Player = board.getPlayerPos();
	 // find path to new position, which is the index of the box stored in the move and the opposite direction
	 // loop over solutionMoves
	 // find path by breadth first search - possible tiles are TileEmpty and TileGoal
	 // we get the shortest path (if there are more paths)
	 // save path into a string
	 // then: applymove and save it, too
	 // then we are on previous position of the 1st box.
	 // next step: find path to next position and so on... don't forget to save :)
	 // do this for all moves that provide the solution

	}



vector<int> Agent::pathSearch(Board board, Pos start, Pos end){ // TODO

	vector<int> path;
	//path.push_back(board.TileIndex(start));

		int current = board.TileIndex(start);
		if(current == board.TileIndex(end))
			return path; // TODO:  we want to return the string to the end-position

		// TODO: find the shortest (?) path from start to end position, returning as a string would be fine
		// if you treat it as a Move you can use one function for ToString
		// so store pos + next dir
	}


bool Agent::dfs(Board board, int depth, vector<Move> &solutionMoves){

		bool foundsolution = false;

		// do we have the solution? yes -> return sequence of moves
		int boxingoal=0;

	for(int j = 0; j < board.getBoxes().size(); j++)
	{
	if(board.getBoard()[board.getBoxes().at(j)]== TileGoal){ // FIXME: doesn't look very good - does this work?
		boxingoal++;
	}
	}
	if(boxingoal == board.getBoxes().size()){  // every box in the goal? - we are done
		return true;
	}


	vector<Move> possibleMoves;

	board.GenerateMoves(possibleMoves);
	for(int i = 0; i < possibleMoves.size(); i++)
	{
		board.ApplyMove(possibleMoves.at(i));
		foundsolution = dfs(board, depth-1, solutionMoves);
		if(foundsolution)
		{
			board.ApplyMove(possibleMoves.at(i));
			foundsolution = dfs(board, depth-1);
			if(foundsolution)
			{
				solutionMoves.push_back(possibleMoves.at(i));
			}
			board.UndoMove(possibleMoves.at(i));
		}

	}

	return foundsolution;
}

string Agent::submitSolution(){
			return mSolution; // TODO
}

 */

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



