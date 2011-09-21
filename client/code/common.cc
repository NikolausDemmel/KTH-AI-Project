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

};
