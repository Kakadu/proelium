#include "GameMap.h"
#include "mapsquare.h"
#include "stuff/consts.h"

GameMap::GameMap(int w, int h,QObject *parent) : QObject(parent) {
    _lastId = 0;
    _width=w;
    _height=h;
    int s  = w+h+2;
    field_mask = new bool*[s];
    _field = new MapSquare**[s];

    for (int i=0;i<s;++i) {
	field_mask[i] = new bool[s];
	_field[i] = new MapSquare*[s];
	for (int j=0; j<s; ++j) {
	    _field[i][j] = NULL;
	    field_mask[i][j] = false;
	}
    }

    for (int i=0;i<s;++i)
	for (int j=0;j<s;++j) {
	    _field[i][j]= NULL;
	    field_mask[i][j] = false;
	}

    //little lines
    for (int i=0; i<h+2; ++i)
	for (int j=0; j<w+1; ++j)
	    field_mask[w+i-j][j+i] = true;
    //large lines
    for (int i=0; i<h+1; ++i)
	for (int j=0; j<w+2; ++j)
	    field_mask[w+1+i-j][j+i] = true;

    for (int i=0;i<s;i++) {
	for (int j=0; j<s; j++) {
	    _field[i][j] = (field_mask[i][j]) ? (new MapSquare()) : NULL;
	}
    }
}
void GameMap::init() {
    int s  = _width+_height+2;
    MapSquare* sq;
    for (int i=0; i<s; ++i)
	for (int j=0; j<s; ++j)
	    if ((sq=_field[i][j]) != NULL) {
		sq->setTerrainSprite(40);
	    }
}

MapSquare* GameMap::getSquare1(int i,int j) {
    int s = _width + _height +2;
    if (i >= s || j >= s)
	return NULL;
    return _field[i][j];
}

bool GameMap::tryMove(Unit *u, Game::Direction dir) {
    int i,j;
    MapSquare* src = locateUnit(i,j,u);
    if (src == NULL) {
        qDebug() << "can't move: mapsquare not found";
        return false;
    }

    int x = i+directions[dir].first;
    int y = j+directions[dir].second;
    MapSquare* dst = getSquare1(x, y);
    if (dst == NULL)
        return false;

    src->removeUnit(u);
    dst->addUnit(u);
    return true;
}
