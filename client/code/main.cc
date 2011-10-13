#include <iostream>
#include <string>
#include "client.h"

using namespace std;


void usage(string program) {
    std::cerr << "usage: " << program << " host port boardnum [playsol] [timeout]" << std::endl;
    std::cerr << "usage: " << program << " -f filename boardnum [playsol] [timeout]"  << std::endl;
}

int main(int pArgC, char* pArgs[])
{
	try {
		if (pArgC < 4 || pArgC > 6)
		{
			usage(pArgs[0]);
			return -1;
		}

		bool playsol = false;
		int timeout = 0;

		if (pArgC >= 5)
		{
			if (string(pArgs[4]) != "playsol") {
				timeout = atoi(pArgs[4]);
			} else {
				playsol = true;
				if (pArgC == 6)
					timeout = atoi(pArgs[5]);
			}
		}

		mnp::Client client;

		client.setPlaySolution(playsol);
		if (timeout > 0)
			client.setTimeout(timeout);

		if(string(pArgs[1]) == "-f") {
			client.initBoardFromFile(pArgs[2], pArgs[3]);
		} else {
			client.initBoardFromServer(pArgs[1], pArgs[2], pArgs[3]);
		}

		client.run();

		return 0;
	}
	catch(const char *e) {
		cout << "[exception] " << e << endl;
	}
	catch(exception &e) {
		cout << "[exception] " << e.what() << endl;
	}
	catch(...) {
		cout << "[exception] Unknown" << endl;
	}

	return -2;
}


