#ifndef RESCONTAINER_H
#define RESCONTAINER_H
#include <QVector>
#include <QtCore>
#include <reshelpers/resloader1.h>
#include "stuff/consts.h"

typedef QVector<QPixmap> Images;

class SpritesPack {
private:
    virtual void f() =0;
};
class DirPack {
    QVector<Images> _c;
public:
    DirPack() {
        for(int i=0; i<10; i++) {
            Images i;
            _c.push_back(i);
        }
        Q_ASSERT(_c.count() == 10);
    }
    Images& operator[](Game::Direction d) {
        if ((d>0) && (d<10) && (d!=5))
            return _c[d];
        throw "Wrong direction!";
    }
};

class UnitPack : public SpritesPack {
    void f() {}
public:
    DirPack attack, death, move;
};

class TerrainPack : public SpritesPack {
    void f() {}
public:
    Images content;
};

#endif // RESCONTAINER_H
