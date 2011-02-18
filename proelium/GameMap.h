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
public:
    bool** field_mask;
    explicit GameMap(int,int,QObject *parent = 0);
    MapSquare* getSquare1(int i,int j);
    int width() { return _width; }
    int height() { return _height; }
    void init();
signals:
    void unitRemoved(int);
public slots:

};

#endif // MAP_H
