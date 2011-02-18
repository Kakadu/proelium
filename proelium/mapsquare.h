#ifndef MAPSQUARE_H
#define MAPSQUARE_H

#include <QObject>
#include "unit.h"
#include <QList>

class MapSquare : public QObject
{
Q_OBJECT
private:
    int _terrainSpriteIndex;
    int _resourceSpriteIndex;   // one square can have only 1 resource;


public:
    explicit MapSquare();
    QList<Unit*> units;
    void setTerrainSprite(int x) { _terrainSpriteIndex=x; }
    int terrainSprite() { return _terrainSpriteIndex; }
    void setResourceIndex(int x) { _resourceSpriteIndex=x; }
    int reousrceIndex() { return _resourceSpriteIndex; }
    void addUnit(Unit*);
signals:

public slots:

};

#endif // MAPSQUARE_H
