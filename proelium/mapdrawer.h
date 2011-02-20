#ifndef MAPDRAWER_H
#define MAPDRAWER_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include "reshelpers/rescontainer.h"
#include "action/action.h"
#include "reshelpers/gametextureitem.h"
#include "GameMap.h"
#include "reshelpers/resloader1.h"

class MapDrawer : public QObject, public UnitVisitor {
    Q_OBJECT
private:
    QGraphicsScene* _scene;
    GameMap* _map;
    QMap<int, GameTextureItem*> unitGraphics;
    int _imageWidth, _imageHeight;
    static const QColor& grayColor;

public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
    void paintField();
    void placeArmies();

    virtual void visit(FireUnitAction& act) {

	QString res = act.result ? "killed" : "fired";
	qDebug() << act.attackerID << " " << res << " " << act.victimID;
	qDebug() << "visited FireUnitAction";
	GameTextureItem* item;
	if (unitGraphics.contains(act.attackerID)) {
	    item = unitGraphics.value(act.attackerID);
	    QObject::connect(item,SIGNAL(animationEnded()),
			     this,SLOT(endVisiting()) );
	    item->animate(act.attackerName);
	} else {

	}
	//emit continueModel();
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
    void endVisiting() {

    }

private slots:
    void wakeUpModel() {
	emit continueModel();
    }
};

#endif // MAPDRAWER_H
