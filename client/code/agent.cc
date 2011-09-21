/*
 * agent.cc
 *
 *  Created on: 21.09.2011
 *      Author: astridrupp
 */

#include "agent.h"

namespace mnp {


void Agent::search(Board board){


}

string Agent::findPath(){

}

int Agent::dfs(Board board, int depth){
	int value;
	vector<Move> moves;
	board.GenerateMoves(moves);
	for(int i = 0; i < moves.size(); i++)
	{
		board.ApplyMove(moves.at(i));
		value = dfs(board, depth-1);
		board.UndoMove(moves.at(i));
	}
}

string Agent::submitSolution(){

}


}; // namespace



