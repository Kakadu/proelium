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
    virtual void visit(FireUnitAction& act) {
	QString res = act.result ? "killed" : "fired";
	qDebug() << act.unitID << " " << res << " " << act.targetID;
	qDebug() << "visited FireUnitAction";
	QGraphicsPixmapItem* item;
	if (unitGraphics.contains(act.targetID)) {
	    item = unitGraphics.take(act.targetID);
	    _scene->removeItem(item);
	} else {

	}
	emit continueModel();
    }
    virtual void visit(MoveUnitAction&) {
	qDebug() << "visited MoveUnitAction";
	emit continueModel();
    }
    virtual void visit(EndWarAction&) {
	qDebug() << "War never ends.";
    }
    virtual void visit(NoAction&) {
	qDebug() << "visited NoAction";
	emit continueModel();
    }

signals:
    void continueModel();

public slots:
    void removeUnit(const int x) {
	unitGraphics.remove(x);
    }
    void applyAction(AbstractUnitAction* u) {
	u->accept(*this);
	delete u;
    }
};

#endif // MAPDRAWER_H
