/*
 * board.cc
 *
 *  Created on: 20.09.2011
 *      Author: astridrupp
 */


#include "board.h"

namespace mnp {

//Construct from file
 Board::Board(const char* fileName, unsigned int boardNumber) {
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
		ParseBoard(stringStream.str().c_str());
//		cout<<"H: "<<mHeight<<" W: "<<mWidth<<endl;
	}

//creates a String that can be printed
string Board::BoardToString(uint8_t printFlags = 0) const
{
	stringstream board;
	for(int y = mHeight-1; y >= 0; --y) {
		for (int x = 0; x < mWidth; x++) {
			int index = TileIndex(x, y);
			board << FlagString(mBoard[index], printFlags);
			if ((mPlayerPos.x == x) && (mPlayerPos.y == y))
				if (mBoard[index] & TileGoal)
					board << '+';
				else
					board << '@';
			else
				board << TileCharacter(mBoard[index]);
			board << EndFlagString(printFlags);
		}
		board << '\n';
	}

	return board.str();
}

	// applies a move, so it only moves a box
void Board::ApplyMove(const Move &move) {
	// set the current tile of the box to the tilegoal or tileempty - so undo the BoxFlag
	// the new tile is in the direction that's stored in the move
	// set the new tile to a tile with a box

	mBoard[TileIndex(move.getBoxPos())] &= (~TileBox);
	mBoard[TileIndex(TileIndex(move.getBoxPos()), move.getMoveDir())] |= TileBox;
	mPlayerPos = move.getBoxPos();
	for(int j = 0; j < mBoxes.size(); j++){
		if(mBoxes.at(j) == TileIndex(move.getBoxPos())){
			mBoxes.at(j) = TileIndex(TileIndex(move.getBoxPos()), move.getMoveDir());
		}
	}


}
	// move the box back
void Board::UndoMove(const Move &move) {
	// set the tile of the move-pos to tilebox
	// the tile in the stored direction should be set to tileempty or tilegoal
	mBoard[TileIndex(move.getBoxPos())] |= TileBox;
	mBoard[TileIndex(TileIndex(move.getBoxPos()), move.getMoveDir())] &= (~TileBox);
	mPlayerPos = Pos(move.getBoxPos(),move.getMoveDir(),-1);

	for(int j = 0; j < mBoxes.size(); j++){
		if(mBoxes.at(j) ==  TileIndex(TileIndex(move.getBoxPos()), move.getMoveDir())){
		mBoxes.at(j) = TileIndex(move.getBoxPos());
		}
	}
}
bool Board::doAction(Dir toWhere) {
	Pos next(mPlayerPos,toWhere);

	if( mBoard[TileIndex(next)] & TileWall) {
		throw "banging in the wall";
	} else if (mBoard[TileIndex(next)] & TileBox) {
		cout<<"pushAt-"<<mPlayerPos.ToString()<<DirToString(toWhere)<<endl;
		Move tMove(Pos(mPlayerPos,toWhere),toWhere);
		ApplyMove(tMove);
		return true;
	} else {
		mPlayerPos=next;
		return false;
	}
}

void Board::undoAction(Dir fromWhere,bool unPush){
	Pos prev(mPlayerPos,fromWhere,-1);
	if(unPush) {
		Move tMove(mPlayerPos,fromWhere);
		UndoMove(tMove);
	} else {
		if(mBoard[TileIndex(prev)] & (TileWall|TileBox)) throw "banging back";
		else mPlayerPos = prev;
	}
}

void Board::simulateActions(const char* actions){
	bool pushed=false;
		switch(*actions) {
		case ' ':
			return simulateActions(++actions);
		case 'U':	case 'u':	case '0':
			cout<<"up"<<endl;
			pushed = doAction(Up);
			PrintBoard();
			simulateActions(++actions);
			undoAction(Up,pushed);
			return;
		case 'D':	case 'd':	case '1':
			cout<<"down"<<endl;
			pushed = doAction(Down);
			PrintBoard();
			simulateActions(++actions);
			undoAction(Down,pushed);
			return;
		case 'L':	case 'l':	case '2':
			cout<<"left"<<endl;
			pushed = doAction(Left);
			PrintBoard();
			simulateActions(++actions);
			undoAction(Left,pushed);
			return;
		case 'R':	case 'r':	case '3':
			cout<<"right"<<endl;
			pushed = doAction(Right);
			PrintBoard();
			simulateActions(++actions);
			undoAction(Right,pushed);
			return;
		case '\0':
			return;
		default:
			cout<<"invalid character in simulate actions: \'\\"<<(int)*actions<<"\'\n";
			return;
		}

	}

// FIXME: would it not be quicker to incrementally update all the reachability information
// also maybe store the boxes as a list of positions
void Board::GenerateMoves(vector<Move> &moves)
{
	VisitTile(TileIndex(mPlayerPos), moves);
#ifdef DEBUG
	cout << BoardToString(TileFlagMask);
	cout << "Possible moves:" << endl;
	for (vector<Move>::iterator iter = moves.begin(); iter != moves.end(); ++iter) {
		cout << iter->ToString() << endl;
	}
#endif
	ClearFlags();
}

void Board::VisitTile(int tileIndex, vector<Move> &moves)
{
	if ( (mBoard[tileIndex] & TileVisitedFlag) == 0 ) {
		mBoard[tileIndex] |= TileVisitedFlag;
		for (int dir = 0; dir < 4; ++dir) {
			int next = TileIndex(tileIndex, static_cast<Dir>(dir));
			if ((mBoard[next] & TileBox) && (TileFree(mBoard[TileIndex(next,static_cast<Dir>(dir))]))) {
				mBoard[tileIndex] |= TileExtraFlag;
				moves.push_back(Move(TileIndexToPos(next), static_cast<Dir>(dir)));
			} else if (TileFree(mBoard[next])) {
				VisitTile(next, moves);
			}
		}
	}
}

void Board::ClearFlags() {
	for (int i = 0; i < mWidth*mHeight; ++i) {
		mBoard[i] &= ~TileFlagMask;
	}
}

bool Board::isSolved() {

	for(int i=0;i<mWidth*mHeight;i++)
		if( (mBoard[i] & TileBox) && !(mBoard[i] & TileGoal) ) return false;

	return true;

}

// TODO: function that takes a move (rather, a position), and generates the steps to execute this move
// _breadth_ first search (to get the shortest possible way to get there).

void Board::ParseBoard(const char* board) {
	mWidth = 0;
	mHeight = 0;

	int currWidth = 0;
	for (int i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			++mHeight;
			mWidth = max(mWidth, currWidth);
			currWidth = 0;
		} else {
			++currWidth;
		}
	}

	mBoard = new uint8_t[mWidth*mHeight];

	int y = mHeight - 1;
	int x = 0;
	for (int i = 0; i < strlen(board); ++i) {
		if (board[i] == '\n') {
			// FIXME: maybe find all unreachable regions and make them "invalid" straight away.
			// for now we just fill trailing tiles with walls.
			for (; x < mWidth; ++x) {
				mBoard[TileIndex(x, y)] = TileWall;
			}
			x = 0;
			--y;
		} else {
			mBoard[TileIndex(x, y)] = ParseTile(board[i]);
			if(board[i] == '$' || board[i] == '*'){
				mBoxes.push_back(TileIndex(x, y));
			}
			if (board[i] == '@' || board[i] == '+') {
				mPlayerPos = Pos(x, y);
			}
			++x;
		}
	}
}

	uint8_t Board::ParseTile(char c) {
		switch(c) {
		case ' ':
			return TileEmpty;
		case '#':
			return TileWall;
		case '$':
			return TileBox | TileEmpty;
		case '.':
			return TileGoal;
		case '+':
			return TileGoal;
		case '*':
			return TileGoal | TileBox;
		case '@':
			return TileEmpty;
		default:
			throw "Invalid tile character.";
		}
	}

	char Board::TileCharacter(uint8_t t) {
		switch(t & (~TileFlagMask)) {
		case TileEmpty:
			return ' ';
		case TileWall:
			return '#';
		case (TileBox | TileEmpty):
			return '$';
		case (TileGoal | TileBox):
			return '*';
		case TileGoal:
			return '.';
		default:
			throw "Invalid tile type.";
		}
	}

const char* Board::FlagString(uint8_t tile, uint8_t flags) {
	if (flags & tile & TileVisitedFlag)
		if (flags & tile & TileExtraFlag)
			return "\e[43m";
		else
			return "\e[42m";
	else
		return "";
}

const char* Board::EndFlagString(uint8_t flags) {
	if (flags)
		return "\e[0m";
	else
		return "";
}



};

