/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include "agent.h"

namespace mnp {


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

string Agent::findPath(){

	/* 1st step: get position of the player.
	 * find path to new position, which is the index of the box stored in the move and the opposite direction
	 * find path by breadth first search - possible tiles are TileEmpty and TileGoal
	 * we get the shortest path (if there are more paths)
	 *
	 *
	 * save path into a string
	 * then: applymove and save it, too
	 * then we are on previous position of the 1st box.
	 * next step: find path to next position and so on... don't forget to save :)
	 * do this for all moves that provide the solution
	 */

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
			solutionMoves.push_back(possibleMoves.at(i));
		}
		board.UndoMove(possibleMoves.at(i));
	}
	return foundsolution;
}

string Agent::submitSolution(){
			return mSolution; // TODO
}


}; // namespace



