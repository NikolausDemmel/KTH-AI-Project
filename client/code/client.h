/*
 * client.h
 *
 *  Created on: 12.10.2011
 *      Author: demmeln
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <signal.h>
#include "board.h"
#include "csocket.h"
#include "agent.h"



namespace mnp {


void timeout_handler (int i) {
	cout << "[timeout] exiting..." << endl;
	cout.flush();
	exit(-3);
}


class Client
{
public:

	Client():
		mBoard(0),
		mSocket(0),
		mPlaySolution(false)
	{
		signal(SIGALRM,timeout_handler);
	}

	~Client()
	{
		if (mBoard)
			delete mBoard;
		if (mSocket)
			delete mSocket;
	}

	void disableTimer() {
		setitimer(ITIMER_REAL, 0, 0);
	}

	void enableTimer(uint seconds) {
		struct itimerval diff;
        diff.it_interval.tv_sec = 0;
        diff.it_interval.tv_usec = 0;
        diff.it_value.tv_sec = seconds;
        diff.it_value.tv_usec = 0;

		setitimer(ITIMER_REAL, &diff, 0);
	}

	void setPlaySolution(bool playsol) {
		mPlaySolution = playsol;
	}

	void setTimeout(uint timeout) {
		mTimeout = timeout;
	}

	void initBoardFromFile(string filename, string board) {
		mBoard = new Board(filename.c_str(), atoi(board.c_str()));
	}

	void initBoardFromServer(string host, string port, string board) {
		mSocket = new soko::CSocket(host, port);

		mSocket->WriteLine(board);

	    string line;
	    stringstream ssBoard;

	    //read number of rows
	    mSocket->ReadLine(line);
	    int lNumRows = atoi(line.c_str());

	    //read each row
	    for (int i = 0; i < lNumRows; i++)
	    {
	    	mSocket->ReadLine(line);
	    	ssBoard << line;
	    	ssBoard << '\n';
	    }

	    mBoard = new Board(ssBoard.str());
	}

	void run()
	{

	    cerr << "[client] running client with board" << endl;
		cerr << mBoard->boardToString();

	    Agent agent;
	    agent.setBoard(mBoard);
        agent.setBackBoard(mBoard);

	    if (mTimeout > 0)
	    	enableTimer(mTimeout);
	    cout << "[client] searching for solution" << endl;
	    agent.findSolution();
	    cout << "[client] found solution" << endl;
		disableTimer();

	    cout << "[client] executing solution" << endl;
	    string solution = agent.executeSolution();

	    if (mPlaySolution) {
		    cout << "[client] play solution: " << solution << endl;
	    	mBoard->simulateActions(solution.c_str());
	    }

	    cout << "[client] initial board: " << endl;
		mBoard->printBoard();
	    cout << "[client] solution: " << solution << endl;

	    if (mSocket) {
	    	cout << "[client] sending solution to server" << endl;
	    	mSocket->WriteLine(solution);

		    string line;
	    	mSocket->ReadLine(line);

	    	cout << "[client] answer from server" << endl;
			cout << line << endl;
	    } else {
	    	// TODO: implement local verification of solution
	    	cout << "[client] local verification of solution not yet implemented" << endl;
	    }
	}

private:

	uint mTimeout;
	Board *mBoard;
	soko::CSocket *mSocket;
	bool mPlaySolution;
};



}


#endif /* CLIENT_H_ */
