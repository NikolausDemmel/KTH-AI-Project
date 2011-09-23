/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include <limits>
#include <algorithm>
#include <queue>
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

void Agent::findSolution() {

	SearchResult result;
	int depth = 1;
//	while(!solutionMoves.empty())
//	solutionMoves.pop();
	solutionMoves.clear();

		do {
#ifdef INFO
			cout << "Trying depth " << depth << endl;
#endif
			result = depthLimitedSearch(depth++);
		} while(result==CutOff);
		myBoard->restoreInitialPlayerPos();
		if (result==Failure) throw "board can't be solved.";
		else return;

}

SearchResult Agent::depthLimitedSearch(int depth){

	// Base Cases
	if ((*myBoard).isSolved()) return Solution;
	if (depth == 0) return CutOff;

	bool cutOff_Occured = false;
	vector<Move> possibleMoves;
	SearchResult result;
	myBoard->GenerateMoves(possibleMoves);
	for(int i = 0; i < possibleMoves.size(); i++) {
		myBoard->ApplyMove(possibleMoves.at(i));
//		myBoard->PrintBoard();
		result = depthLimitedSearch(depth-1);
		myBoard->UndoMove(possibleMoves.at(i));

		if(result==CutOff) cutOff_Occured = true;
		else if (result!=Failure) {
			//solutionMoves.push(possibleMoves.at(i)); //NOTE using a stack not a list
			solutionMoves.push_front(possibleMoves.at(i));
			return Solution;
		}	//else : solution not found, try next node :/
	}
	if(cutOff_Occured) return CutOff;
	else return Failure;

}

void Agent::setBoard(Board *aBoard) {
	myBoard = aBoard;
}

struct TileNode {

	TileNode():
		distance(numeric_limits<int>::max()),
		visited(false),
		parent(Up)
	{
	}

	int distance;
	bool visited;
	Dir parent;

};

struct TileIndexComparator {

	TileIndexComparator(const vector<TileNode> *nodes):
		mNodes(nodes)
	{
	}

	bool operator()(const int &a, const int &b) {
		return mNodes->at(a).distance < mNodes->at(b).distance;
	}

private:
	const vector<TileNode> *mNodes;

};

bool Agent::shortestPathSearch(string &actions, const Board &board, Pos start, Pos end)
{
	stringstream ss;

	vector<TileNode> nodes(board.size());

	int startIndex = board.TileIndex(start);
	int endIndex = board.TileIndex(end);

	nodes[startIndex].distance = 0;
	nodes[startIndex].visited = true;

	TileIndexComparator comparator(&nodes);
	priority_queue<int, vector<int>, TileIndexComparator> pq(comparator);
	pq.push(startIndex);

	while(!pq.empty()) {
		int curr = pq.top();
		pq.pop();
		if(curr == endIndex) {
			while(curr != startIndex) {
				ss << directionToAction(invertDirection(nodes[curr].parent));
				curr = board.TileIndex(curr, nodes[curr].parent);
			}
			actions = ss.str();
			reverse(actions.begin(), actions.end());
			return true;
		}
		for (int i = 0; i < 4; ++i) {
			Dir dir = static_cast<Dir>(i);
			int next = board.TileIndex(curr, dir);
			if (!nodes[next].visited && isTileFree(board.getTile(next))) {
				nodes[next].distance = nodes[curr].distance + 1;
				nodes[next].parent = invertDirection(dir);
				nodes[next].visited = true;
				pq.push(next);
			}
		}
	}

	return false;
}

bool Agent::actionsForMove(string &actions, const Board &board, Move &move)
{
	if (shortestPathSearch(actions, board, board.getPlayerPos(), move.getPlayerPos())) {
		actions += directionToAction(move.getMoveDir());
		return true;
	}
	return false;
}

string Agent::executeSolution() {

	// FIXME: should we really do this here
	// FIXME: ^ we can do it after each method is completed. (for instance, in the function body of find solutions)
	//myBoard->restoreInitialPlayerPos();

#ifdef INFO
	cout << "Execution Solution." << endl;
#endif
	stringstream solution;
	list<Move>::iterator solutionItr = solutionMoves.begin();
	for(;solutionItr!=solutionMoves.end();solutionItr++) {
		Move move = *solutionItr;
//	while(!solutionMoves.empty()) {
//		Move move = solutionMoves.top();
//		solutionMoves.pop();
		string actions;
		actionsForMove(actions, *myBoard, move);
		solution <<  actions;
		myBoard->ApplyMove(move);
#ifdef INFO
		cout << "Applying move " << move.ToString() << endl;
		cout << myBoard->BoardToString();
#endif
	}
	// FIXME: big questions: I think we need to maintain an invariant that after anything agent does to a board, the board remains the same?
	// I(yg) made a function simulate actions that takes in the string of solution and runs them on a board *BUT* also undoes them.
	//
	while(solutionItr!=solutionMoves.begin()) {
		solutionItr--;
#ifdef INFO
		cout<<"Undoing move "<<solutionItr->ToString()<<endl;
#endif

		myBoard->UndoMove(*solutionItr);
	}
	myBoard->restoreInitialPlayerPos();
	return solution.str();
}

}; // namespace



