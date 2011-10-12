/*
 * tilenode.h
 *
 *  Created on: 11.10.2011
 *      Author: demmeln
 */

#ifndef TILENODE_H_
#define TILENODE_H_


#include <limits>
#include "definitions.h"


namespace mnp {


struct TileNode {

	TileNode():
		distance(numeric_limits<uint>::max()),
		visited(false),
		parent(Up)
	{
	}

	uint distance;
	bool visited;
	Dir parent;

};


}


#endif /* TILENODE_H_ */
