/*
 * common.cc
 *
 *  Created on: 21.09.2011
 *      Author: demmeln
 */

#include "common.h"

namespace mnp {

const char* DirToString(Dir dir)
{
	switch(dir) {
	case Up:
		return "Up";
	case Down:
		return "Down";
	case Left:
		return "Left";
	case Right:
		return "Right";
	default:
		throw "Unreachable!";
	}
}

Dir invertDirection(Dir dir) {
	switch(dir) {
	case Up:
		return Down;
	case Down:
		return Up;
	case Left:
		return Right;
	case Right:
		return Left;
	}
}

char directionToAction(Dir dir) {
	switch(dir) {
	case Up:
		return 'U';
	case Down:
		return 'D';
	case Left:
		return 'L';
	case Right:
		return 'R';
	}
}

int manhattanDistance(Pos a, Pos b) {
	return (abs( (int) ((int)a.x-a.y))+(b.x-b.y) );
}

};
