/*
 * board.cc
 *
 *  Created on: 20.09.2011
 *      Author: astridrupp
 */


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


void Board::printBoard(uint8_t printFlags) const {
	cout << boardToString(printFlags) << endl;
}

//creates a String that can be printed
string Board::boardToString(uint8_t printFlags, const vector<TileNode> * const nodes) const
{
	stringstream board;
	for(int y = mHeight-1; y >= 0; --y) {
		for (int x = 0; x < mWidth; x++) {
			index_t index = tileIndex(x, y);
			board << flagString(mTiles[index], printFlags);
			if (mPlayerIndex == index)
				if (mTiles[index].isGoal())
					board << '+';
				else
					board << '@';
			else
				board << tileCharacter(mTiles[index], (nodes ? &(nodes->at(index)) : 0));
			board << endFlagString(printFlags);
		}
		board << '\n';
	}

	return board.str();
}

// applies a move, so it only moves a box
void Board::applyMove(const Move &move) {

	index_t curr = move.getBoxIndex();
	index_t next = move.getNextIndex(this);

	updateHash(getZobristBox(curr));
	updateHash(getZobristPlayer(mPlayerIndex));

	int box = mTiles[curr].removeBox();
	mTiles[next].setBox(box);
	setPlayerIndex(curr);

	updateHash(getZobristBox(next));
	updateHash(getZobristPlayer(mPlayerIndex));

	mMissingGoals += (mTiles[curr].isGoal() ? 1 : 0);
	mMissingGoals -= (mTiles[next].isGoal() ? 1 : 0);

	// FIXME
//	for (vector<int>::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it) {
//		if (*it == curr) {
//			*it = next;
//		}
//	}
}

// move the box back
void Board::undoMove(const Move &move) {

	index_t curr = move.getBoxIndex();
	index_t next = move.getNextIndex(this);

	updateHash(getZobristBox(next));
	updateHash(getZobristPlayer(mPlayerIndex));

	int box = mTiles[next].removeBox();
	mTiles[curr].setBox(box);
	setPlayerIndex(move.getPlayerIndex(this));

	updateHash(getZobristBox(curr));
	updateHash(getZobristPlayer(mPlayerIndex));

	mMissingGoals -= (mTiles[curr].isGoal() ? 1 : 0);
	mMissingGoals += (mTiles[next].isGoal() ? 1 : 0);

	// FIXME:
//	for (vector<int>::iterator it = mBoxes.begin(); it != mBoxes.end(); ++it) {
//		if (*it == next) {
//			*it = curr;
//		}
//	}
}

// TODO: refactor
//bool Board::doAction(Dir toWhere) {
//	Pos next(mPlayerPos,toWhere);
//
//	if( mBoard[TileIndex(next)] & TileWall) {
//		throw "banging in the wall";
//	} else if (mBoard[TileIndex(next)] & TileBox) {
//		cout<<"pushAt-"<<mPlayerPos.ToString()<<DirToString(toWhere)<<endl;
//		Move tMove(Pos(mPlayerPos,toWhere),toWhere);
//		ApplyMove(tMove);
//		return true;
//	} else {
//		setPlayerPos(next);
//		return false;
//	}
//}
//
//void Board::undoAction(Dir fromWhere,bool unPush){
//	Pos prev(mPlayerPos,fromWhere,-1);
//	if(unPush) {
//		Move tMove(mPlayerPos,fromWhere);
//		UndoMove(tMove);
//	} else {
//		if(mBoard[TileIndex(prev)] & (TileWall|TileBox)) throw "banging back";
//		else setPlayerPos(prev);
//	}
//}
//
//void Board::simulateActions(const char* actions){
//	bool pushed=false;
//		switch(*actions) {
//		case ' ':
//			return simulateActions(++actions);
//		case 'U':	case 'u':	case '0':
//			cout<<"up"<<endl;
//			pushed = doAction(Up);
//			PrintBoard();
//#ifdef INFO
//			cin.get();
//#endif
//			simulateActions(++actions);
//			undoAction(Up,pushed);
//			return;
//		case 'D':	case 'd':	case '1':
//			cout<<"down"<<endl;
//			pushed = doAction(Down);
//			PrintBoard();
//#ifdef INFO
//			cin.get();
//#endif
//			simulateActions(++actions);
//			undoAction(Down,pushed);
//			return;
//		case 'L':	case 'l':	case '2':
//			cout<<"left"<<endl;
//			pushed = doAction(Left);
//			PrintBoard();
//#ifdef INFO
//			cin.get();
//#endif
//			simulateActions(++actions);
//			undoAction(Left,pushed);
//			return;
//		case 'R':	case 'r':	case '3':
//			cout<<"right"<<endl;
//			pushed = doAction(Right);
//			PrintBoard();
//#ifdef INFO
//			cin.get();
//#endif
//			simulateActions(++actions);
//			undoAction(Right,pushed);
//			return;
//		case '\0':
//			return;
//		default:
//			cout<<"invalid character in simulate actions: \'\\"<<(int)*actions<<"\'\n";
//			return;
//		}
//
//	}

// FIXME: would it not be quicker to incrementally update all the reachability information
// also maybe store the boxes as a list of positions
void Board::generateMoves(vector<Move> &moves)
{
	visitTile(mPlayerIndex, moves);
#ifdef VERBOSE_GENERATE_MOVES
	cout << boardToString(Tile::VisitedFlag | Tile::ExtraFlag);
	cout << "Possible moves:" << endl;
	for (vector<Move>::iterator iter = moves.begin(); iter != moves.end(); ++iter) {
		cout << iter->toString(this) << endl;
	}
#endif
	clearFlags();
}

void Board::visitTile(index_t tile, vector<Move> &moves)
{
	if ( !(mTiles[tile].flagsSet(Tile::VisitedFlag)) ) {
		mTiles[tile].setFlags(Tile::VisitedFlag);
		foreach (Dir dir, cDirs) {
			index_t next = tileIndex(tile, dir);
			if (mTiles[next].isBox() && mTiles[tileIndex(next, dir)].isFree()) {
				mTiles[tile].setFlags(Tile::ExtraFlag);
				moves.push_back(Move(next, dir));
			} else if (mTiles[next].isFree()) {
				visitTile(next, moves);
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
		throw "board of size 0 ???";
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
				mTiles[tile] = mBoxes.size(); // set the number of the box to the index on the mBoxes list
				mBoxes.push_back(tile);
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
char Board::tileCharacter(const Tile &tile, const TileNode * const node) {
	switch(tile.static_type) {
	case Tile::Empty:
		if (tile.isBox())
			return '$';
		else {
			if (node)
				return '0' + node->distance;
			else
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
		if( (mTiles[i].isBox()) && !(mTiles[i].isGoal()) )
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


};

