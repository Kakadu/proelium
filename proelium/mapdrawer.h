#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QMap>
#include <QAnimationGroup>
#include <QPoint>
#include <QMessageBox>
#include "reshelpers/rescontainer.h"
#include "action/action.h"
#include "reshelpers/gametextureitem.h"
#include "GameMap.h"
#include "reshelpers/resloader1.h"
#include "reshelpers/anigroup.h"
#include "GlobalConst.h"

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
    AniGroup* aniGroup;

public:
    MapDrawer(QGraphicsScene* sc, GameMap* m);
    void repaint();
    void paintField();
    void placeArmies(ModelParam*);

    virtual void visit(NewUnitsAppearedAction&) {
        qDebug() << "NewUnitsAppearedAction";
	repaint();
	endVisiting();
    }

    virtual void visit(FireUnitAction& act) {
          qDebug() << "FireUnitAction";
	QString res = act.result ? "killed" : "fired";
	qDebug() << act.attackerID << " " << res << " " << act.victimID;

	GameTextureItem *itemFire = NULL, *itemDeath = NULL;
	aniGroup->clear();
	if (unitGraphics.contains(act.attackerID))
	    itemFire = unitGraphics.value(act.attackerID);
	Q_ASSERT(itemFire != NULL);
	aniGroup->addAnimationSeq(itemFire->animate(act));
	RemoveTimer* timer = NULL;

	if (act.result) {
	    // victim can still alive
	    if (unitGraphics.contains(act.victimID))
		itemDeath = unitGraphics.value(act.victimID);
	    Q_ASSERT(itemDeath != NULL);
	    aniGroup->addAnimationSeq(itemDeath->animateDeath(act.victimName));

	    timer = new RemoveTimer(this,itemDeath);
	    QObject::connect(timer, SIGNAL(timeout2(GameTextureItem*)),
			     this, SLOT(setDeathPixmap(GameTextureItem*)));
	}
	aniGroup->startSeq(timer);
    }

    virtual void visit(MoveUnitAction& act) {
        qDebug() << "MoveUnitAction";
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
	    item->animate(act,-1,_map,newCoords);
	    aniGroup->addAnimationPar(item->animationGroup());
	    aniGroup->startPar(NULL);
	}else {
	    qDebug() << "Unit with id " << act.unit()->id << "not found. continue";
	    endVisiting();
	}
    }

    virtual void visit(EndWarAction& act) {
	QMessageBox msg;
	msg.setText(act.message);

	msg.exec();
	qDebug() << "War never ends.";
    }
    virtual void visit(NoAction&) {
        qDebug() << "NoAction";
        wakeUpModel();
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
	aniGroup->clear();
	qDebug() << "end visiting";
	wakeUpModel();	
    }
    void setDeathPixmap(GameTextureItem* item) {
	item->setDeathSprite();
    }

    /*void removePixmap(GameTextureItem* item) {
	_scene->removeItem(item);
    } */

private slots:
    void wakeUpModel() {
	emit continueModel();
    }
};

#endif // MAPDRAWER_H
