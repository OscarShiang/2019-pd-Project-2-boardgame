#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsItemGroup>
#include "tile.h"

class Board: public QGraphicsItemGroup {
public:
    Board();

    Tile *tile[8][8];
};

#endif // BOARD_H
