#ifndef MAPDRAWER_H
#define MAPDRAWER_H
#include <QObject>
#include <QGraphicsScene>
#include "GameMap.h"
#include "reshelpers/resloader1.h"
#include <QPixmap>
#include <QVector>
#include "action/action.h"

static QVector<QPixmap> TerrainSprites;

class MapDrawer : public QObject, public UnitVisitor {
    Q_OBJECT
private:
    QGraphicsScene* _scene;
    GameMap* _map;

    int _imageWidth, _imageHeight;
public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
    virtual void visit(FireUnitAction&) {}
    virtual void visit(MoveUnitAction&) {}
signals:

public slots:

};

#endif // MAPDRAWER_H
