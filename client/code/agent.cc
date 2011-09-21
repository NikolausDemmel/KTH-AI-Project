/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include "agent.h"

namespace mnp {


void Agent::search(Board board){
	int val = dfs(board, 2);

}

string Agent::findPath(){

}

int Agent::dfs(Board board, int depth){

	bool foundsolution = false;

	// do we have the solution? yes -> return sequence of moves
	int boxingoal=0;

	for(int j = 0; j < board.getBoxes().size(); j++)
	{
	if(board.getBoard()[board.getBoxes().at(j)]== TileGoal){ // FIXME: doesn't look very good - does this work?
		boxingoal++;
	}
	}
	if(boxingoal == board.getBoxes().size())  // every box in the goal? - we are done
		foundsolution = true;

	vector<Move> possibleMoves;
	vector<Move> solutionMoves;
	board.GenerateMoves(possibleMoves);
	for(int i = 0; i < possibleMoves.size(); i++)
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

string Agent::submitSolution(){

}


}; // namespace



