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

	SearchResult resultForward, resultBackward;
	uint depth = 1;
	uint64_t hashMeeting = 0;

	typedef pair<uint64_t, uint64_t> hashpair;

	mSolutionMoves.clear();
	vector<index_t> possiblePlayerInd, forwardpos;
	vector<hashpair> backwardhashes,forwardhashes;


	// get all hashes for playerpos at beginning of backward search
	mBoard->getAllPlayerPosHashForward(forwardpos, forwardhashes, true);
	foreach(hashpair hashf, forwardhashes) {
		mHashTable.lookup(hashf.first, 0, hashf.second); //depth = 0?
		cout << " put hashes in forwardhash " << hashf.first << ", " << hashf.second <<  endl;
	}
	mBackBoard.getAllPlayerPosHashBack(possiblePlayerInd, backwardhashes, true);
	foreach(hashpair hashb, backwardhashes){
		mBackwardHashTable.lookup(hashb.first,0, hashb.second); //depth = 0?
		cout << " put hashes in backwardhash " << hashb.first << ", " << hashb.second <<  endl;
	}

	do {
#ifdef INFO
		cout << "[agent] trying depth " << depth << endl;
		cout << "[agent] # of boxes " << mBoard->mBoxes.size() << endl;
		cout << "[agent] log size of board " << mBoard->mIndexBits << endl;
#endif

		resultForward = depthLimitedSearch(depth, mBoard, Forward, hashMeeting);
		resultBackward = depthLimitedSearch(depth, &mBackBoard, Backward, hashMeeting);
		++depth;
#ifdef INFO
		cout << "[agent] search result for this depth: forward: " << cSearchResultNames[resultForward] << " ; backward: " << cSearchResultNames[resultBackward] << endl;
		mHashTable.printStatistics();
		mBackwardHashTable.printStatistics();
#endif
		mHashTable.clearStatistic();
		mBackwardHashTable.clearStatistic();
	} while (resultForward == CutOff);


	cout << "After search" << endl;
	foreach(Move &m, mSolutionMoves) {
		cout << m.toString(mBoard);
	}
	mBoard->restoreInitialPlayerIndex();

#ifdef INFO
	cout << "Result of search was: forward: " << cSearchResultNames[resultForward] << " ; backward: " << cSearchResultNames[resultBackward] << endl;
#endif

	if (resultForward==Failure || resultBackward==Failure)
		throw "board can't be solved."; // Fail more grance-fully
	else
		return;
}


SearchResult Agent::depthLimitedSearch(uint depth, Board *board, SearchType type, uint64_t &hashMeeting)
{


	// 1. Base Cases
	if (depth == 0){
		return CutOff;
	}

	//if (type == Forward && mBoard->isSolved()) return Solution; //FIXME: if type is backward, return "Solution" if boxes are at initial positions
																//(should not happen, because forward and backward should meet somewhere before)

	// 2. Hashtable lookup
	if (type ==  Forward) {

		if (mBackwardHashTable.compare(mBoard->getHash(), mBoard->computeHash2Value())) {
			mHashTable.lookup(mBoard->getHash(), depth, mBoard->computeHash2Value()); // FIXME: necessary?
			cout << "ForwardHash: " << mBoard->getHash() << " at index " << mBoard->getPlayerIndex() << endl;
			cout << "ForwardHash computed: " << mBoard->computeHashValue() << endl;
			cout << "ForwardHash 2: " << mBoard->computeHash2Value() << endl;
			hashMeeting = mBoard->computeHash2Value();
			return SolutionMeeting;
		}


		if (mHashTable.lookup(mBoard->getHash(), depth, mBoard->computeHash2Value())){
			return CutOff; // FIXME: do we need to store CutOff vs Failure in the hash table?
		}

	}
	else {

		if (hashMeeting != 0) {
			if(hashMeeting == mBackBoard.computeHash2Value()){
				cout << "hashmeeting ok, index is " << mBackBoard.getPlayerIndex() << endl;
				cout << "hashmeeting  " << hashMeeting << endl;
				cout << "BackwardHash in hashmeeting: " << mBackBoard.getHash() << " at index " << mBackBoard.getPlayerIndex() << endl;
				cout << "BackwardHash computed in hashmeeting: " << mBackBoard.computeHashValue() << endl;
				cout << "BackwardHash 2 computed in hashmeeting: " << mBackBoard.computeHash2Value() << endl;
				return SolutionMeeting;
			}
		}

		if (mBackwardHashTable.lookup(mBackBoard.getHash(), depth, mBackBoard.computeHash2Value())) {
			return CutOff; // FIXME: do we need to store CutOff vs Failure in the hash table?
		}
	}

	// 3. Deadlock detection
	// TODO: Deadlock detection should kick in here:



	// 4. Search one step deeper
	bool cutoff_occured = false;
	vector<Move> possible_moves;
	SearchResult result;

	board->generateMoves(possible_moves, type);
	foreach ( Move &move, possible_moves) {

		//cout << "apply move with playerindex " << board->getPlayerIndex() << " hash " << board->getHash() << endl;
		board->applyMove(move, type);
//		myBoard->PrintBoard();
		//cout << "applied move with playerindex " << board->getPlayerIndex() << " hash " << board->getHash() << endl;
		result = depthLimitedSearch(depth-1, board, type, hashMeeting);
		board->undoMove(move, type);
		//cout << "undone move with playerindex " << board->getPlayerIndex() << " hash " << board->getHash() << endl;



		if (result==CutOff) {
			cutoff_occured = true;
		} else if (result != Failure) {
			if (type == Forward) {
				mSolutionMoves.push_front(move);
			}
			if (result == SolutionMeeting && type == Backward) {
				move.setMoveDir(invertDirection(move.getMoveDir()));
				move.setBoxIndex(board->tileIndex(move.getBoxIndex(), move.getMoveDir(), -1));
				mSolutionMoves.push_back(move);
				//return SolutionMeeting;
			}
			return SolutionMeeting; //FIXME: return result?
		}
	}

	// 5. assess outcome
	if (cutoff_occured) return CutOff;
	else return Failure;

}

void Agent::setBoard(Board *board) {
	mBoard = board;
}

void Agent::setBackBoard(Board *board) {
	mBackBoard = *board;
	mBackBoard.mBoxes.clear();

	int boxIndex = 0;
	for (index_t i = 0; i < mBackBoard.mSize; ++i) {
		if (mBackBoard.mTiles[i].isBox()) {
			mBackBoard.mTiles[i].removeBox();
		}
		if (mBackBoard.mTiles[i].isGoal()) {
			mBackBoard.mTiles[i].setBox(boxIndex);
			mBackBoard.mBoxes.push_back(i);
			boxIndex++;
		}
	}
	mBackBoard.mPlayerIndex = 0;
	mBackBoard.recomputeHashValue();

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

		mBoard->applyMove(move, Forward);

#ifdef INFO

		cout << "Applying move "<< endl;
		cout << "Applying move "<< move.toString(mBoard) << endl;
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
		mBoard->undoMove(*move_iter, Forward);
	}
	mBoard->restoreInitialPlayerIndex();

	return solution.str();
}

}; // namespace



