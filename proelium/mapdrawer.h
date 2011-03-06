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
	//qDebug() << "visited FireUnitAction";
	GameTextureItem* item;
	if (unitGraphics.contains(act.attackerID)) {
	    item = unitGraphics.value(act.attackerID);
	    QObject::disconnect(item,SIGNAL(animationEnded(Invoker*)),
				this,SLOT(endVisiting(Invoker*)) );
	    QObject::connect(item,SIGNAL(animationEnded(Invoker*)),
			     this,SLOT(endVisiting(Invoker*)) );
	    item->animate(act, act.attackerName, NULL);
	}
	//emit continueModel();
    }

    virtual void visit(MoveUnitAction& act) {
	qDebug() << act.unit()->id << " moves";
	//qDebug() << "visited MoveUnitAction";
	Unit* u = act.unit();
	int path = act.unit()->nonEvaledPath();
	if (path < 25) {
	    emit continueModel();
	    return;
	}

	int i,j;
	_map->locateUnit(i,j,act.unit());
	if (i==-1 || j==-1) {
	    throw "pizdets";	
	}
	_map->getSquare1(i,j)->removeUnit(act.unit());
	qDebug() << "let's move on ("<<i<<", "<<j<<")";
		;
	MapSquare* sq;
	if ((sq = _map->getSquare1(i,j+1)) != NULL) {
	    sq->addUnit(u);
	    qDebug() << "unit moved to (i,j+1)";
	    //GameTextureItem* item = unitGraphics[u->id];
	} else
	if ((sq = _map->getSquare1(i+1,j)) != NULL) {
	    sq->addUnit(u);
	    qDebug() << "unit moved to (i+1,j6)";
	    //GameTextureItem* item = unitGraphics[u->id];
	} else {
	    //qDebug() << "can't move: i = " << i << " and j = "<< j;
	}
	repaint();
	//qDebug() << "unit moved";
	emit continueModel();
/*	MoveUnitAction act2(act);
	act2.unit()->afterEvalPath(25);

	class aaa : public Invoker {
	    UnitVisitor* _v;
	    MoveUnitAction _act;
	public:
	    aaa(MoveUnitAction& a, UnitVisitor* v) : _v(v),_act(a) {}
	    virtual void invoke() {
		_v->visit(_act);
	    }
	};

	GameTextureItem* item;
	if (unitGraphics.contains(act.unit()->id)) {
	    item = unitGraphics.value(act.unit()->id);
	    QObject::connect(item,SIGNAL(animationEnded(Invoker*)),
			     this,SLOT(endVisiting(Invoker*)) );
	    aaa* a = new aaa(act2,this);
	    item->animate(MOVE, act.unit()->name,a);
	} */
    }
    virtual void visit(EndWarAction&) {
	//qDebug() << "War never ends.";
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
	u->accept(*this);
	delete u;
    }
    void endVisiting(Invoker* inv) {
	qDebug() << "end visiting";
	if (inv == NULL) {
	    wakeUpModel();
	    return;
	} else {

	    inv->invoke();
	    delete inv;
	}
    }

private slots:
    void wakeUpModel() {
	emit continueModel();
    }
};

#endif // MAPDRAWER_H
