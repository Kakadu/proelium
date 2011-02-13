#ifndef MAPSQUARE_H
#define MAPSQUARE_H

#include <QObject>

class MapSquare : public QObject
{
Q_OBJECT
private:
    int _terrainSpriteIndex;
    int _resourceSpriteIndex;   // one square can have only 1 resource;
public:
    explicit MapSquare();
    void setTerrainSprite(int x) { _terrainSpriteIndex=x; }
    int terrainSprite() { return _terrainSpriteIndex; }
    void setResourceIndex(int x) { _resourceSpriteIndex=x; }
    int reousrceIndex() { return _resourceSpriteIndex; }

signals:

public slots:

};

#endif // MAPSQUARE_H
