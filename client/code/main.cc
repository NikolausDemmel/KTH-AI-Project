#include <iostream>
#include <cstdlib>
#include "csocket.h"
#include "board.h"
#include "agent.h"

using namespace std;
using namespace mnp;

int main(int pArgC, char* pArgs[])
{
    if(pArgC<4)
    {
        std::cerr << "usage: " << pArgs[0] << " host port boardnum [mode = contest]" << std::endl;
        std::cerr << "usage: " << pArgs[0] << " -f filename boardnum [mode = play]"  << std::endl;
        return -1;
    }

//    if(string(pArgs[1]) == "-f") {
//
//    } else {

    soko::CSocket lSocket(pArgs[1],pArgs[2]);

    lSocket.WriteLine(pArgs[3]);
    
    std::string lLine;
    string strBoard;

    //read number of rows
    lSocket.ReadLine(lLine);
    int lNumRows=atoi(lLine.c_str());
    
    //read each row
    for(int i=0;i<lNumRows;i++)
    {
        lSocket.ReadLine(lLine);
        strBoard += lLine;
        strBoard += '\n';
    }
    
    vector<Move> moves;

    //Board myBoard("board.txt",1);
    //Board myBoard("#####\n# . #\n#   #\n#  #\n# $.#\n#@  #\n#####\n");
    Board board(strBoard);

    board.printBoard();

    Agent myAgent;
    myAgent.setBoard(&board);
    myAgent.findSolution();

//
//    cout << myAgent.solutionMoves.size() << " moves." << endl;
//    myBoard.PrintBoard();
//
//    while (myAgent.solutionMoves.size()>0) {
//    	myBoard.ApplyMove(myAgent.solutionMoves.top());
//    	cout<<"moves left: "<<myAgent.solutionMoves.size()<<endl;
//    	myBoard.PrintBoard();
//    	myAgent.solutionMoves.pop();
//    }

    board.printBoard();
    cout << "[main] execute solution" << endl;
    string solution = myAgent.executeSolution();
    cout << "[main] solution found" << endl;
    board.printBoard();

    board.simulateActions(solution.c_str());

    cout << "[main] actions: " << solution << endl;

    string lMySol = solution;


    //we've found our solution (this is actually the solution to board 1)    
	//std::string lMySol("U R R U U L D L L U L L D R R R R L D D R U R U D L L U R");
	//these formats are also valid:
	//std::string lMySol("URRUULDLLULLDRRRRLDDRURUDLLUR");
    //std::string lMySol("0 3 3 0 0 2 1 2 2 0 2 2 1 3 3 3 3 2 1 1 3 0 3 0 1 2 2 0 3");

    //send the solution to the server
    //myBoard.simulateActions("URRUULDLLULLDRRRRLDDRURUDLLUR");
    lSocket.WriteLine(lMySol);
    
    //read answer from the server
    lSocket.ReadLine(lLine);
    
    std::cout << lLine << std::endl;

    return 0;
}


