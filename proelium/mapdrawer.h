#ifndef MAPDRAWER_H
#define MAPDRAWER_H
#include <QObject>
#include <QGraphicsScene>
#include "GameMap.h"
#include "reshelpers/resloader1.h"
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <reshelpers/rescontainer.h>
#include "action/action.h"

static QMap<QString, SpritesPack*> Sprites;

class MapDrawer : public QObject, public UnitVisitor {
    Q_OBJECT
private:
    QGraphicsScene* _scene;
    GameMap* _map;
    QMap<int, QGraphicsPixmapItem*> unitGraphics;
    int _imageWidth, _imageHeight;
    static const QColor& grayColor;
public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
    void paintField();
    void placeArmies();
    virtual void visit(FireUnitAction&) {}
    virtual void visit(MoveUnitAction&) {}
signals:

public slots:
    void removeUnit(const int x) {
	unitGraphics.remove(x);
    }
};

#endif // MAPDRAWER_H
