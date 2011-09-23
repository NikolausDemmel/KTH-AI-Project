/*
 * hash_table.h
 *
 *  Created on: 23.09.2011
 *      Author: demmeln
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "common.h"
#include <cassert>
#include <cstring>

namespace mnp {

typedef uint64_t ht_entry_t;

class HashTable
{
public:
	HashTable(int sizeInBits) {
		assert(sizeInBits > 0);
		assert(sizeInBits <= 63); // even 63 will be much too big anyway, but never mind
		mSize = 1 << sizeInBits;
		mHashMask = mSize-1;
		mTable = new uint64_t[mSize];
		mDepthTable = new uint8_t[mSize];
		clear();
	}

	void clear() {
#ifdef INFO
		mMisses = 0;
		mHits = 0;
		mMissesBecauseOverlap = 0;
		mMissesBecauseZero = 0;
#endif
		memset(mTable, 0, mSize*sizeof(uint64_t));
		memset(mDepthTable, 0, mSize*sizeof(uint8_t));
	}

	uint64_t getIndex(uint64_t hash) const {
		return hash & mHashMask;
	}

	uint64_t getValue(uint64_t hash) const {
		return mTable[getIndex(hash)];
	}

	uint8_t getDepthValue(uint64_t hash) const {
		return mDepthTable[getIndex(hash)];
	}

	bool lookup(uint64_t hash, uint8_t remaining_depth, uint64_t validation_hash = 0) {
//#ifdef DEBUG
//		cout << "lookup on hash: " << hash << endl;
//#endif
		if (hash == 0) {
			// this is bad, size 0 is our "Not there" value

#ifdef INFO
			++mMissesBecauseZero;
#endif
			return false;
		}
		uint64_t index = getIndex(hash);
		uint64_t value = mTable[index];
		if (validation_hash == 0)
			validation_hash = hash;

		if (value == 0) {
#ifdef INFO
			++mMisses;
#endif
			mTable[index] = validation_hash;
			mDepthTable[index] = remaining_depth;
			return false;
		}
		if (value == validation_hash) {
			if (mDepthTable[index] < remaining_depth) {
				mDepthTable[index] = remaining_depth;
#ifdef INFO
				++mMissesBecauseDepth;
#endif
				return false;
			}
#ifdef INFO
			++mHits;
#endif
			return true;
		}
		// TODO: implement some strategy to deal with collisions.
#ifdef INFO
		++mMissesBecauseOverlap;
#endif
		return false;
	}

#ifdef INFO
	void printStatistics() {
		cout << "Hash table statistics:" << endl;
		cout << "Misses: " << mMisses << endl;
		cout << "Hits: " << mHits << endl;
		cout << "Misses due to Overlap: " << mMissesBecauseOverlap << endl;
		cout << "Misses due to Zero hash: " << mMissesBecauseZero << endl;
		cout << "Misses due to insufficient depth: " << mMissesBecauseDepth << endl;
	}
#endif

private:
	uint64_t *mTable;
	uint8_t *mDepthTable;
	uint64_t mSize;
	uint64_t mHashMask;

#ifdef INFO
	uint64_t mMisses;
	uint64_t mHits;
	uint64_t mMissesBecauseOverlap;
	uint64_t mMissesBecauseZero;
	uint64_t mMissesBecauseDepth;
#endif

};

};


#endif /* HASH_TABLE_H_ */
