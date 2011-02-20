#ifndef MAP_H
#define MAP_H
#include <QVector>
#include <QVector2D>
#include <QObject>
#include "mapsquare.h"

class GameMap : public QObject {
    Q_OBJECT
private:
    int _width, _height;
    MapSquare*** _field;
    inline int s() { return 2+_width+_height; }

public:
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    int width() { return _width; }
    int height() { return _height; }
    void init();
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

signals:
    void unitRemoved(int);
public slots:

};

#endif // MAP_H
