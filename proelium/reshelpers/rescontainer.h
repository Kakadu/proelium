#ifndef RESCONTAINER_H
#define RESCONTAINER_H
#include <QMap>
#include <QVector>
#include <QPixmap>
#include <QtCore>
#include <QObject>
#include <QString>
#include <reshelpers/resloader1.h>


typedef QVector<QPixmap> Images;
class SpritesPack {
private:
    virtual void f() =0;

};
class UnitPack : public SpritesPack {
    void f() {}
public:
    Images attack,death,move,normal;
};
class TerrainPack : public SpritesPack {
    void f(){}
public:
    Images content;
};



#endif // RESCONTAINER_H
