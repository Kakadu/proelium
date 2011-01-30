#ifndef MAPDRAWER_H
#define MAPDRAWER_H
#include <QObject>
#include <QGraphicsScene>
#include "GameMap.h"
#include "reshelpers/terrainitem.h"

class MapDrawer : public QObject {
Q_OBJECT
private:
    QGraphicsScene* _scene;
    GameMap* _map;
    TerrainItem* _sprites;
public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
signals:

public slots:

};

#endif // MAPDRAWER_H
