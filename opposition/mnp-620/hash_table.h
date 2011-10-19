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
#include <iomanip>

namespace mnp {

typedef uint64_t ht_entry_t;

class HashTable
{
public:
	HashTable(uint sizeInBits) {
		assert(sizeInBits > 0);
		assert(sizeInBits <= 63); // even 63 will be much too big anyway, but never mind
		mSizeInBits = sizeInBits;
		mSize = 1 << sizeInBits;
		mHashMask = mSize-1;
		mTable = new uint64_t[mSize];
		mRemainingDepthTable = new uint16_t[mSize];
		mDepthTable = new uint16_t[mSize];
		clear();
	}

#ifdef INFO
	void clearStatistic() {
		mMisses = 0;
		mCollisionMisses = 0;
		mHits = 0;
		mCollisionHits = 0;
		mCollision = 0;
		mMissesBecauseOverlap = 0;
		mMissesBecauseZero = 0;
		mMissesBecauseDepth = 0;
		mCollisionMissesBecauseDepth = 0;
	}
#endif

	void clear() {
#ifdef INFO
		clearStatistic();
		mEntries = 0;
		mCollisionEntries = 0;
#endif
		memset(mTable, 0, mSize*sizeof(uint64_t));
		memset(mRemainingDepthTable, 0, mSize*sizeof(uint16_t));
		memset(mDepthTable, 0, mSize*sizeof(uint16_t));
	}

	uint64_t getIndex(uint64_t hash) const {
		return hash & mHashMask;
	}

	uint64_t getValue(uint64_t hash) const {
		return mTable[getIndex(hash)];
	}

	uint64_t lookupDepthValue(uint64_t hash) const {
		uint64_t index = getIndex(hash);
		uint64_t value = mTable[index];

		uint64_t validation_hash = hash;

		if (value == 0) {
			return 0;
		}
		if (value == validation_hash) {
			return getDepthValue(index);
		}

		int64_t new_index;

		for (uint i = 1;; i++) {
			if (i + mSizeInBits > 64) {
				return 0;
			}
			new_index = getIndex(index >> i);
			if (mTable[new_index] == 0) {
				return 0;
			}
			value = mTable[new_index];
			if (value == validation_hash) {
				return getDepthValue(new_index);
			}
		}
	}

	uint16_t getRemainingDepthValue(uint64_t hash) const {
		return mRemainingDepthTable[getIndex(hash)];
	}

	uint16_t getDepthValue(uint64_t hash) const {
		return mDepthTable[getIndex(hash)];
	}

	bool lookup(uint64_t hash, uint16_t remaining_depth, uint16_t current_depth, uint64_t validation_hash = 0) {
//#ifdef DEBUG
//		cout << "lookup on hash: " << hash << endl;
//#endif
		if (hash == 0) {
			// this is bad, 0 is our "Not there" value
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
			++mEntries;
			++mMisses;
#endif
			mTable[index] = validation_hash;
			mRemainingDepthTable[index] = remaining_depth;
			if (current_depth > 0)
				mDepthTable[index] = current_depth;
			return false;
		}
		if (value == validation_hash) {
			if (mRemainingDepthTable[index] < remaining_depth) {
				mRemainingDepthTable[index] = remaining_depth;
				if (current_depth > 0)
					mDepthTable[index] = current_depth;
				//mDepthTable[index] = current_depth;
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

		// collision occurred, find other position (at the moment with quadratic probing)
		// TODO: think of other collision resolution
		// IDEA: use the remaining bits from hash, e.g. try getIndex(hash >> 10), then getIndex(hash >> 20), etc
#ifdef INFO
		++mCollision;

//		cout << "freie Plaetze: " << mSize-mEntries << endl;
//		cout << "storedHash " << value << endl;
//		cout << "newHash " << validation_hash << endl;
#endif
		int64_t new_index;

		for (uint i = 1;; i++) {
			if (i + mSizeInBits > 64) {
#ifdef INFO
				++mMissesBecauseOverlap;
#endif
				return false;
			}
			new_index = getIndex(index >> i);
			//cout << "neuer Index: " << new_index << endl;
			if (mTable[new_index] == 0) {
#ifdef INFO
				++mCollisionMisses;
				++mCollisionEntries;
#endif
				mTable[new_index] = validation_hash;
				mRemainingDepthTable[new_index] = remaining_depth;
				if (current_depth > 0)
					mDepthTable[new_index] = current_depth;
				//cout << "stored" << endl;
				return false;
			}
			value = mTable[new_index];
			if (value == validation_hash) {
				if (mRemainingDepthTable[new_index] < remaining_depth) {
					mRemainingDepthTable[new_index] = remaining_depth;
					if (current_depth > 0)
						mDepthTable[new_index] = current_depth;
					//mDepthTable[new_index] = current_depth;
#ifdef INFO
					++mCollisionMissesBecauseDepth;
#endif
					return false;
				}
#ifdef INFO
				++mCollisionHits;
				//cout << "same hash" << endl;
#endif
				return true;
			}
		}

		return false;
	}

	bool compare(uint64_t hash, uint64_t validation_hash = 0) {
		uint64_t index = getIndex(hash);
		uint64_t value = mTable[index];
		if (validation_hash == 0)
			validation_hash = hash;

		if (value == 0) {
			return false;
		}
		if (value == validation_hash) {
			return true;
		}

		int64_t new_index;

		for (uint i = 1;; i++) {
			if (i + mSizeInBits > 64) {
				return false;
			}
			new_index = getIndex(index >> i);
			//cout << "neuer Index: " << new_index << endl;
			if (mTable[new_index] == 0) {
				return false;
			}
			value = mTable[new_index];
			if (value == validation_hash) {
				return true;
			}
		}
	}

#ifdef INFO
	void printStatistics() {
		cout << "Hash table statistics:" << endl;
		cout << setw(40) << "Size: " << setw(10) << mSize << endl;
		cout << setw(40) << "Entries: " << setw(10) << mEntries << endl;
		cout << setw(40) << "CollisionEntries: " << setw(10) << mCollisionEntries << endl;
		cout << setw(40) << "Filled (in %): " << setw(10) << (100*(mEntries+mCollisionEntries)) / mSize << endl;
		cout << setw(40) << "Misses: " << setw(10) << mMisses << endl;
		cout << setw(40) << "CollisionMisses: " << setw(10) << mCollisionMisses << endl;
		cout << setw(40) << "Hits: " << setw(10) << mHits << endl;
		cout << setw(40) << "CollisionHits: " << setw(10) << mCollisionHits << endl;
		cout << setw(40) << "Collisions: " << setw(10) << mCollision << endl;
		cout << setw(40) << "Misses due to Overlap: " << setw(10) << mMissesBecauseOverlap << endl;
		cout << setw(40) << "Misses due to Zero hash: " << setw(10) << mMissesBecauseZero << endl;
		cout << setw(40) << "Misses due to insufficient depth: " << setw(10) << mMissesBecauseDepth << endl;
		cout << setw(40) << "CollisionMisses due to insufficient depth: " << setw(10) << mCollisionMissesBecauseDepth << endl;
	}
#endif

private:
	uint64_t *mTable;
	uint16_t *mRemainingDepthTable;
	uint16_t *mDepthTable;
	uint64_t mSize;
	uint mSizeInBits;
	uint64_t mHashMask;

#ifdef INFO
	uint64_t mEntries;
	uint64_t mMisses;
	uint64_t mHits;
	uint64_t mCollision;
	uint64_t mMissesBecauseOverlap;
	uint64_t mMissesBecauseZero;
	uint64_t mMissesBecauseDepth;
	uint64_t mCollisionEntries;
	uint64_t mCollisionMisses;
	uint64_t mCollisionHits;
	uint64_t mCollisionMissesBecauseDepth;
#endif

};

};


#endif /* HASH_TABLE_H_ */
