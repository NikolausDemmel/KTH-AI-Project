/*
 * board.cc
 *
 *  Created on: 20.09.2011
 *      Author: astridrupp
 */

#include <algorithm>
#include <sstream>
#include <queue>
#include <vector>
#include "board.h"

namespace mnp {

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION
////////////////////////////////////////////////////////////////////////////////

// FIXME: make sure all members are set up properly by parseBoard

Board::Board(const char* board) {
	parseBoard(board);
}

Board::Board(string board) {
	parseBoard(board.c_str());
}

Board::Board(const char* fileName, unsigned int boardNumber) {
	parseBoardFromFile(fileName, boardNumber);
}

Board::~Board() {
	// FIXME
}

void Board::parseBoardFromFile(const char *fileName, size_t boardNumber) {
	string str;
	ifstream fileStream(fileName);
	ostringstream stringStream;
	while(boardNumber>0) {
		do {
			getline(fileStream,str);
		} while(str.at(0)!=';');
		boardNumber--;
	}
	getline(fileStream,str);
	while(str.at(0)!=';') {
		stringStream<<str<<"\n";
		getline(fileStream,str);
	}
	parseBoard(stringStream.str().c_str());
//		cout<<"H: "<<mHeight<<" W: "<<mWidth<<endl;
}


void Board::printBoard(uint8_t print_flags) const {
	cout << boardToString(print_flags) << endl;
}

//creates a String that can be printed
string Board::boardToString(uint8_t printFlags, const vector<TileGraphNode> * const nodes, bool print_dead) const
{
	stringstream board;
	for(int y = mHeight-1; y >= 0; --y) {
		for (uint x = 0; x < mWidth; ++x) {
			index_t index = tileIndex(x, y);
			board << flagString(mTiles[index], printFlags);
			if (mPlayerIndex == index)
				if (mTiles[index].isGoal())
					board << '+';
				else
					board << '@';
			else
				board << tileCharacter(mTiles[index], (nodes ? &(nodes->at(index)) : 0), print_dead);
			board << endFlagString(printFlags);
		}
		board << '\n';
	}

	return board.str();
}

// applies a move, so it only moves a box
void Board::applyMove(const Move &move, SearchType type = Forward) {

	index_t curr = move.getBoxIndex();
	index_t next = move.getNextIndex(this);

	updateHash(getZobristBox(curr));
	updateHash(getZobristPlayer(mPlayerIndex));

	int box = mTiles[curr].removeBox();
	mTiles[next].setBox(box);
	mBoxes[box] = next;

	if (type == Forward) {
		setPlayerIndex(curr);
		mMissingGoals += (mTiles[curr].isGoal() ? 1 : 0);
		mMissingGoals -= (mTiles[next].isGoal() ? 1 : 0);
	}
	else {
		setPlayerIndex(tileIndex(next, move.getMoveDir(), 1));
	}

	updateHash(getZobristBox(next));
	updateHash(getZobristPlayer(mPlayerIndex));


	// FIXME
//	for (vector<int>::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it) {
//		if (*it == curr) {
//			*it = next;
//		}
//	}
}


// move the box back
void Board::undoMove(const Move &move, SearchType type = Forward) {

	index_t curr = move.getBoxIndex();
	index_t next = move.getNextIndex(this);

	updateHash(getZobristBox(next));
	updateHash(getZobristPlayer(mPlayerIndex));

	int box = mTiles[next].removeBox();
	mTiles[curr].setBox(box);
	mBoxes[box] = curr;

	if (type == Forward) {
		setPlayerIndex(move.getPlayerIndex(this));
		mMissingGoals -= (mTiles[curr].isGoal() ? 1 : 0);
		mMissingGoals += (mTiles[next].isGoal() ? 1 : 0);
	}
	else {
		setPlayerIndex(next);
	}

	updateHash(getZobristBox(curr));
	updateHash(getZobristPlayer(mPlayerIndex));


	// FIXME:
//	for (vector<int>::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it) {
//		if (*it == next) {
//			*it = curr;
//		}
//	}
}

bool Board::doAction(Dir to) {
	index_t next = tileIndex(mPlayerIndex, to);

	if( mTiles[next].isWall() ) {
		throw "[board] banging head in the wall :-/";
	} else if (mTiles[next].isBox() ) {
		cout << "push at " << indexToPos(mPlayerIndex).toString() << cDirs[to] << endl;
		Move move(next, to);
		applyMove(move, Forward);
		return true;
	} else {
		setPlayerIndex(next);
		return false;
	}
}

void Board::undoAction(Dir from, bool unpush){
	index_t prev = tileIndex(mPlayerIndex, from, -1);
	if (unpush) {
		Move move(mPlayerIndex, from);
		undoMove(move, Forward);
	} else {
		if (!mTiles[prev].isFree()) throw "[board] banging back in the wall :-/";
		else setPlayerIndex(prev);
	}
}

void Board::simulateActions(const char* actions){
	bool pushed = false;
	switch(*actions) {
	case ' ':
		return simulateActions(++actions);
	case 'U':	case 'u':	case '0':
		cout << "up" << endl;
		pushed = doAction(Up);
		printBoard();
#ifdef INFO
		cin.get();
#endif
		simulateActions(++actions);
		undoAction(Up, pushed);
		return;
	case 'D':	case 'd':	case '1':
		cout << "down" << endl;
		pushed = doAction(Down);
		printBoard();
#ifdef INFO
		cin.get();
#endif
		simulateActions(++actions);
		undoAction(Down, pushed);
		return;
	case 'L':	case 'l':	case '2':
		cout << "left" << endl;
		pushed = doAction(Left);
		printBoard();
#ifdef INFO
		cin.get();
#endif
		simulateActions(++actions);
		undoAction(Left, pushed);
		return;
	case 'R':	case 'r':	case '3':
		cout << "right" << endl;
		pushed = doAction(Right);
		printBoard();
#ifdef INFO
		cin.get();
#endif
		simulateActions(++actions);
		undoAction(Right, pushed);
		return;
	case '\0':
		return;
	default:
		cout << "[board] invalid character in simulate actions: \'\\" << (int)*actions << "\'\n";
		return;
	}
}


// FIXME: would it not be quicker to incrementally update all the reachability information
// also maybe store the boxes as a list of positions
void Board::generateMoves(vector<Move> &moves, SearchType type)
{
	if (type == Forward) {
		visitTile(mPlayerIndex, moves);
		//cout << "size of all moves forward" << moves.size() << endl;
	}
	else {

		//first iteration: do not use start pos of player...
		vector<index_t> possiblePlayerInd;
		vector<uint64_t> hashes;
		if(mPlayerIndex == 0)
		{
			//cout << "should only happen at the beginning!" << endl;
			getAllPlayerPosHashBack(possiblePlayerInd, hashes, false);
			//cout << "size of possible player indices " << possiblePlayerInd.size() << endl;
			foreach(index_t tile, possiblePlayerInd){
				vector<Move> tempmoves;
				reverseVisitTile(tile, tempmoves);  // works quite good with our flags, moves are only once in the vector :)

				//cout << "size of tempmoves " << tempmoves.size() << endl;
				foreach(Move move, tempmoves){
					moves.push_back(move);
				}
				//cout << "size of all moves " << moves.size() << endl;
			}
		}
		else{
			reverseVisitTile(mPlayerIndex, moves);
			//cout << "playerindex:" << mPlayerIndex << endl;
			//cout << "size of all moves in else " << moves.size() << endl;
		}


	}

#ifdef VERBOSE_GENERATE_MOVES
	cout << boardToString(Tile::VisitedFlag | Tile::ExtraFlag);
	cout << "Possible moves:" << endl;
	for (vector<Move>::iterator iter = moves.begin(); iter != moves.end(); ++iter) {
		cout << iter->toString(this) << endl;
	}
#endif
	clearFlags();
}


void Board::getAllPlayerPosHashForward(vector<index_t> &possiblePlayerInd, vector<uint64_t> &hashes, bool hash){

	updateHash(getZobristPlayer(mPlayerIndex));
	vector<Move> moves;
	visitTile(mPlayerIndex, moves);
	clearFlags();
	//if(mPlayerIndex == mInitialPlayerIndex)
	//{
		foreach(Move move, moves)
	{
			index_t next = tileIndex(move.getBoxIndex(), move.getMoveDir(), -1);
			possiblePlayerInd.push_back(next);
			//cout << "possible player index pushback forward" << next << endl;
			if(hash == true)
			{
				setPlayerIndex(next);
				updateHash(getZobristPlayer(next));
				//cout << "pushback hash forward" << getHash() <<endl;
				hashes.push_back(getHash());
				updateHash(getZobristPlayer(next));
			}
	}
		setPlayerIndex(mInitialPlayerIndex);
	//}

	updateHash(getZobristPlayer(mPlayerIndex));

}
void Board::getAllPlayerPosHashBack(vector<index_t> &possiblePlayerInd, vector<uint64_t> &hashes, bool hash){
	//if(mPlayerIndex == -1){
	//cout << "before removing startindex" << getHash() <<endl;
		updateHash(getZobristPlayer(mPlayerIndex));
		//cout << "after removing startindex" << getHash() <<endl;
	//}

	for(int i = 0; i<mTiles.size();i++)
				{
					if(mTiles[i].isGoal()){
						foreach(Dir dir, cDirs){
							index_t next = tileIndex(i, dir);
							if(mTiles[next].isFree()){
								possiblePlayerInd.push_back(next); // TODO: do not save same player pos several times
								//cout << "possible player index pushback back" << next << endl;
								if(hash == true){
									setPlayerIndex(next);
									updateHash(getZobristPlayer(mPlayerIndex));
									//cout << "pushback hash back " << getHash() <<endl;
									hashes.push_back(getHash());
									updateHash(getZobristPlayer(mPlayerIndex));
								}
							}
						}
					}
				}
		setPlayerIndex(0);
		//cout << "before adding startindex" << getHash() <<endl;
		updateHash(getZobristPlayer(mPlayerIndex));
		//cout << "after adding startindex" << getHash() <<endl;

}

void Board::visitTile(index_t tile, vector<Move> &moves)
{
	if ( !(mTiles[tile].flagsSet(Tile::VisitedFlag)) ) {
		mTiles[tile].setFlags(Tile::VisitedFlag);
		foreach (Dir dir, cDirs) {
			index_t next = tileIndex(tile, dir);
			if (mTiles[next].isBox() && mTiles[tileIndex(next, dir)].isFree() && !mTiles[tileIndex(next, dir)].isDead()) {
				mTiles[tile].setFlags(Tile::ExtraFlag);
				moves.push_back(Move(next, dir));
			} else if (mTiles[next].isFree()) {
				visitTile(next, moves);
			}
		}
	}
}

void Board::reverseVisitTile(index_t tile, vector<Move> &moves)
{

	if ( !(mTiles[tile].flagsSet(Tile::VisitedFlag)) ) {
		mTiles[tile].setFlags(Tile::VisitedFlag);
		foreach (Dir dir, cDirs) {
			index_t next = tileIndex(tile, dir);
			if (mTiles[next].isBox() && mTiles[tileIndex(tile, invertDirection(dir))].isFree() && !mTiles[tileIndex(tile, invertDirection(dir))].isDead()) {
				mTiles[tile].setFlags(Tile::ExtraFlag);
				moves.push_back(Move(next, invertDirection(dir)));
			} else if (mTiles[next].isFree()) {
				reverseVisitTile(next, moves);
			}
		}
	}
}

void Board::clearFlags() {
	for (uint i = 0; i < mSize; ++i) {
		mTiles[i].clearFlags();
	}
}

bool Board::isSolved() {
	return mMissingGoals == 0;
}

void Board::parseBoard(const char* board) {

	// TODO: detect malformed boards and throw error

	mBoxes.clear();
	mGoals.clear();
	mWidth = 0;
	mHeight = 0;

	uint currWidth = 0;
	for (uint i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			++mHeight;
			mWidth = max(mWidth, currWidth);
			currWidth = 0;
		} else {
			++currWidth;
		}
	}

	mSize = mWidth*mHeight;
	mTiles.resize(mSize);

	if (mSize == 0) {
		throw "[board] board of size 0 ???";
	}

	uint y = mHeight - 1;
	uint x = 0;
	for (uint i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			// FIXME: maybe find all unreachable regions and make them "invalid" straight away.
			// for now we just fill trailing tiles with walls.
			for (; x < mWidth; ++x) {
				mTiles[tileIndex(x, y)].static_type = Tile::Wall;
			}
			x = 0;
			--y;
		} else {
			index_t tile = tileIndex(x, y);
			mTiles[tile] = parseTile(board[i]);
			if (board[i] == '$' || board[i] == '*') {
				mTiles[tile].box = mBoxes.size(); // set the number of the box to the index on the mBoxes list
				mBoxes.push_back(tile);
			}
			if (board[i] == '.' || board[i] == '+' || board[i] == '*') {
				mGoals.push_back(tile);
			}
			if (board[i] == '@' || board[i] == '+') {
				setPlayerIndex(tile);
			}
			++x;
		}
	}

	// FIXME: compute group info

	initializeZobrist();

	mHashValue = computeHashValue();
	mMissingGoals = countMissingGoals();
	mInitialPlayerIndex = mPlayerIndex;
	mIndexBits = calculateIndexBits(mSize);
}

uint Board::calculateIndexBits(uint size) {
	return ceil(log2((float)size));
}

// tile from character ignoring the player and the boxes
Tile Board::parseTile(char c) {
	switch(c) {
	case ' ':
	case '$':
	case '@':
		return Tile(Tile::Empty);
	case '#':
		return Tile(Tile::Wall);
	case '.':
	case '+':
	case '*':
		return Tile(Tile::Goal);
	default:
		throw "Invalid tile character.";
	}
}

// character from tile ignoring the player
char Board::tileCharacter(const Tile &tile, const TileGraphNode * const node, bool print_dead) {
	switch(tile.static_type) {
	case Tile::Empty:
		if (tile.isBox())
			return '$';
		else {
			if (node)
				return '0' + node->distance;
			else if(print_dead) {
				if(tile.isDead())
					return '-';
				else
					return ' ';
			} else
				return ' ';
		}
	case Tile::Wall:
		return '#';
	case Tile::Goal:
		if (tile.isBox())
			return '*';
		else
			return '.';
	default:
		throw "Invalid tile type.";
	}
}

const char* Board::flagString(const Tile &tile, uint8_t flags) {
	if ((Tile::VisitedFlag & flags) && tile.flagsSet(Tile::VisitedFlag))
		if ((Tile::ExtraFlag & flags) && tile.flagsSet(Tile::ExtraFlag))
			return "\e[43m";
		else
			return "\e[42m";
	else
		return "";
}

const char* Board::endFlagString(uint8_t flags) {
	if (flags)
		return "\e[0m";
	else
		return "";
}


uint Board::countMissingGoals() const {
	uint count = 0;
	for(index_t i=0; i < mSize; ++i)
		if( (mTiles[i].isGoal()) && !(mTiles[i].isBox()) )
			++count;
	return count;
}

void Board::initializeZobrist() {
	mZobristBoxes.clear();
	mZobristPlayer.clear();
	for(index_t i = 0; i < mSize; ++i) {
		mZobristBoxes.push_back(rand64());
		mZobristPlayer.push_back(rand64());
	}
}

// get the ZobristNumber of the tile at a certain index.
uint64_t Board::getZobristBox(index_t tile) const {
	if (mTiles[tile].isBox())
		return mZobristBoxes[tile];
	else
		return 0;
}

uint64_t Board::getZobristPlayer(index_t tile) const {
	if (tile == mPlayerIndex)
		return mZobristPlayer[tile];
	else
		return 0;
}

// xor the number to the hash value
void Board::updateHash(uint64_t zobristNumber) {
	mHashValue ^= zobristNumber;
}



// computes the hash value from scratch
uint64_t Board::computeHashValue() const {
	uint64_t hash = 0;
	for (index_t i = 0; i < mSize; ++i) {
		hash ^= getZobristBox(i);
		hash ^= getZobristPlayer(i);
	}
	return hash;
}

// computes an alternative hash value from scratch
uint64_t  Board::computeHash2Value() {
	uint64_t hash = mPlayerIndex;
	foreach (index_t box_index, mBoxes ) {
		hash <<= mIndexBits;
		hash |= box_index;
	}
	return hash; // will always be != 0
}


void Board::setPlayerIndex(index_t player) {
	mPlayerIndex = player;
}

void Board::restoreInitialPlayerIndex() {
	setPlayerIndex(mInitialPlayerIndex);
}


bool Board::shortestPathSearch(string &actions, index_t start, index_t end) const
{
	// This uses dijkstra to search for the end
	// Since we have this very special layout we don't ever need to update a node.

	stringstream ss;

	vector<TileGraphNode> nodes(mSize);

	nodes[start].distance = 0;
	nodes[start].visited = true;

	TileGraphNode::IndexComparator comparator(&nodes);
	priority_queue<uint, vector<uint>, TileGraphNode::IndexComparator> pq(comparator);
	pq.push(start);

	while(!pq.empty()) {
		index_t curr = pq.top();
		pq.pop();
		if (curr == end) {
			// Found the goal, now back up and record the way
			while (curr != start) {
				ss << directionToAction(invertDirection(nodes[curr].parent));
				curr = tileIndex(curr, nodes[curr].parent);
			}
			actions = ss.str();
			reverse(actions.begin(), actions.end());

#ifdef VERBOSE_SHORTEST_PATH
			cout << "Shortest path search result: " << endl;
			cout << boardToString(0, &nodes) << endl;
#endif

			return true;
		}

		// not found the goal yet, so search further
		foreach (Dir dir, cDirs) {
			index_t next = tileIndex(curr, dir);
			if (!nodes[next].visited && mTiles[next].isFree()) {
				nodes[next].distance = nodes[curr].distance + 1;
				nodes[next].parent = invertDirection(dir);
				nodes[next].visited = true;
				pq.push(next);
			}
		}
	}


	return false;
}

bool Board::actionsForMove(string &actions, const Move &move) const
{
	if ( shortestPathSearch(actions, getPlayerIndex(), move.getPlayerIndex(this)) ) {
		actions += directionToAction(move.getMoveDir());
		return true;
	}
	return false;
}



};

