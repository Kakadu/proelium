#ifndef MAPDRAWER_H
#define MAPDRAWER_H
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <QPoint>
#include "reshelpers/rescontainer.h"
#include "action/action.h"
#include "reshelpers/gametextureitem.h"
#include "GameMap.h"
#include "reshelpers/resloader1.h"

class MapDrawer : public QObject, public UnitVisitor {
    Q_OBJECT
private:
    int LEFT_OFFSET, TOP_OFFSET;
    QGraphicsScene* _scene;
    GameMap* _map;
    QMap<int, GameTextureItem*> unitGraphics;
    int _imageWidth, _imageHeight;
    static const QColor& grayColor;
    QPoint screenCoords(int,int);
public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
    void paintField();
    void placeArmies();

    virtual void visit(FireUnitAction& act) {
	QString res = act.result ? "killed" : "fired";
	qDebug() << act.attackerID << " " << res << " " << act.victimID;
	GameTextureItem* item;
	if (unitGraphics.contains(act.attackerID)) {
	    item = unitGraphics.value(act.attackerID);
	    QObject::disconnect(item,SIGNAL(animationEnded(Invoker*)),
				this,SLOT(endVisiting(Invoker*)) );
	    QObject::connect(item,SIGNAL(animationEnded(Invoker*)),
			     this,SLOT(endVisiting(Invoker*)) );
	    item->animate(act, act.attackerName, NULL);

	} else {
	    qDebug() << "Unit with id " << act.attackerID << "not found. continue";
	    endVisiting(NULL);
	}	
    }

    virtual void visit(MoveUnitAction& act) {
	/*
	  Тут логика сейчас представляется мне такой. Танк двигает из одной
	  клетки во вторую. Мы двигаем спрайт по прямой, одновременно сменяя кадры.
	  Т.е. показываем две анимации параллельно.
	  */
	qDebug() << act.unit()->id << " moves";
	GameTextureItem* item;
	if (unitGraphics.contains(act.unit()->id)) {
	    item = unitGraphics.value(act.unit()->id);
	    QPoint newCoords = screenCoords(act.x(), act.y());
	    QObject::disconnect(item,SIGNAL(animationEnded(Invoker*)),
				this,SLOT(endVisiting(Invoker*)) );
	    QObject::connect(item,SIGNAL(animationEnded(Invoker*)),
			     this,SLOT(endVisiting(Invoker*)) );
	    item->animate(act,-1,_map,newCoords);
	}else {
	    qDebug() << "Unit with id " << act.unit()->id << "not found. continue";
	    endVisiting(NULL);
	}
    }
    virtual void visit(EndWarAction&) {
	qDebug() << "War never ends.";
    }
    virtual void visit(NoAction&) {
	//qDebug() << "visited NoAction";
	emit continueModel();
    }

signals:
    void continueModel();

public slots:
    void removeUnit(const int x) {
	unitGraphics.remove(x);
    }
    void applyAction(AbstractUnitAction* u) {
	qDebug() << u;
	MoveUnitAction* mact = dynamic_cast<MoveUnitAction*>(u);
	u->accept(*this);
	delete u;
    }
    void endVisiting(Invoker*) {
	qDebug() << "end visiting";
	    wakeUpModel();
	    return;
    }

private slots:
    void wakeUpModel() {
	emit continueModel();
    }
};

#endif // MAPDRAWER_H
