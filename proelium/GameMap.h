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
    int _lastId;
public:
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    int width() { return _width; }
    int height() { return _height; }
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
    int nextId() {
        return _lastId++;
    }
    /** get string number */
    inline int getStrN(void) {
        double w = this->width();
        return 2*w+3;
    }

    inline int getStrM(int M) {
        return (M+1)/2;
    }

    /** get number of 1st column  */
    inline int get1stColumn(int N) {
        return _width +1 - (N+1)/2;
    }

    inline void getCoords(int& i, int& j, int N, int x) {
        int s = getStrM(N);
        int k = get1stColumn(N);
        i = s+x;
        j  = k+x;
    }

signals:
    void unitRemoved(int);
public slots:

};

#endif // MAP_H
