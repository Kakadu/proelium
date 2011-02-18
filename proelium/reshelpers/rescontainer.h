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
/*
static QMap<QString,SpritesPack> ResourceMap;
class ResContainer
{
private:
    ResContainer(){}
public:
    static void LoadResources() {
	TerrainPack mainTerrain;
	ResLoader1::load1(mainTerrain.content,QString("main_terrain.pcx"),9,9,QColor(255,0,255));
	//ResourceMap.insert(QString("terrain1"),mainTerrain);
    }
};
*/
#endif // RESCONTAINER_H
