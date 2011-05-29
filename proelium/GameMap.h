#ifndef MAP_H
#define MAP_H
#include <QVector>
#include <QVector2D>
#include <QObject>
#include "mapsquare.h"

namespace Game {
    enum Direction {
        Dir_SW = 1,
        Dir_S, Dir_SE,
        Dir_W = 4,
        Dir_E = 6,
        Dir_NW, Dir_N, Dir_NE
    };
}

class GameMap : public QObject {
    Q_OBJECT
private:
    int _width, _height;
    MapSquare*** _field;
    inline int s() { return 2+_width+_height; }
    int _lastId;
public:
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    inline int width() { return _width; }
    inline int height() { return _height; }
    void init();

    MapSquare* locateUnit(int& i, int& j, Unit* uu) {
	MapSquare* sq;
	for (i=0; i<s(); ++i)
	    for (j=0; j<s(); ++j) {
	    if ((sq = _field[i][j]) != NULL)
		foreach (Unit* u, sq->units)
		    if (u->id == uu->id)
			return sq;
	}
	return NULL;
    }

    Unit* findUnit(int id) {
	MapSquare* sq;
	for (int i=0; i<s(); ++i)
	    for (int j=0; j<s(); ++j) {
	    if ((sq = _field[i][j]) != NULL)
		foreach (Unit* u, sq->units)
		    if (u->id == id)
			return u;
	}
	return NULL;
    }
    int inline nextId() {
        return _lastId++;
    }
    bool f() { return false; }
    bool tryMove(Unit*, Game::Direction);
signals:
    void unitRemoved(int);

};

#endif // MAP_H
