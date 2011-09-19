/*
 * move.h
 *
 *  Created on: 19.09.2011
 *      Author: demmeln
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "common.h"

namespace mnp {

class Move
{
public:
	Move(const Pos &pos, Dir dir):
		mBoxPos(pos), mDirection(dir) {
	}

private:
	Pos mBoxPos;
	Dir mDirection;
};

};


#endif /* MOVE_H_ */
