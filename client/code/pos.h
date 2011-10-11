/*
 * pos.h
 *
 *  Created on: 11.10.2011
 *      Author: demmeln
 */

#ifndef POS_H_
#define POS_H_


#include "definitions.h"


namespace mnp {


class Pos
{
public:
	Pos(coord_t x_, coord_t y_) {
		x = x_;
		y = y_;

	}

	Pos() {
		x = 0;
		y = 0;
	}

	Pos(index_t index, const Board* board) {
		x = board->indexToX(index);
		y = board->indexToY(index);
	}

	// pass -1 as invert to get the inverse direction, pass 2 to offset 2 tiles in the given direction etc...
	Pos(Pos currentPos, Dir offset, int factor = 1) {
		x = currentPos.x;
		y = currentPos.y;
		switch (offset) {
		case Up:
			y+=1*factor;
			break;
		case Down:
			y-=1*factor;
			break;
		case Left:
			x-=1*factor;
			break;
		case Right:
			x+=1*factor;
			break;
		}
	}

	string toString() const {
		stringstream ss;
		ss << "(" << x << "," << y << ")";
		return ss.str();
	}

	coord_t x, y;
};


}


#endif /* POS_H_ */
