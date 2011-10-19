/*
 * tilenode.h
 *
 *  Created on: 11.10.2011
 *      Author: demmeln
 */

#ifndef TILENODE_H_
#define TILENODE_H_


#include <limits>
#include <vector>
#include "definitions.h"


namespace mnp {


struct TileGraphNode {

	TileGraphNode():
		distance(numeric_limits<uint>::max()),
		visited(false),
		parent(Up)
	{
	}

	uint distance;
	bool visited;
	Dir parent;

	struct IndexComparator {

		IndexComparator(const vector<TileGraphNode> *nodes):
			mNodes(nodes)
		{
		}

		bool operator()(const uint &a, const uint &b) {
			return mNodes->at(a).distance > mNodes->at(b).distance;
		}

	private:
		const vector<TileGraphNode> *mNodes;

	};

};


}


#endif /* TILENODE_H_ */
