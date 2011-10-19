#include "pos.h"
#include "board.h"


namespace mnp {

Pos::Pos(index_t index, const Board* board) {
	x = board->indexToX(index);
	y = board->indexToY(index);
}


}


