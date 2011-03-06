#ifndef GAMETEXTUREITEM_H
#define GAMETEXTUREITEM_H

#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include "rescontainer.h"
#include "action/action.h"
#include "GameMap.h"

extern QMap<QString, SpritesPack*> Sprites;

typedef void (*cb_type)(void);

class Invoker {
public:
    virtual void invoke() = 0;
};
enum AnimationType {
    ATTACK, MOVE, DEATH
};

class GameTextureItem : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
    Q_PROPERTY(int curSprite READ curSprite WRITE setCurSprite);
    int DURATION;
    QPropertyAnimation* animHelper;
    QPropertyAnimation* animHelperMove;
    QParallelAnimationGroup* aniGroup;
    uint _curSprite;
    UnitPack* curPack;
    QVector<QPixmap> sprites;
    Invoker* _invoker;
    AbstractUnitAction* _lastAct;
    //-----------------------

public:
    explicit GameTextureItem(QGraphicsScene*);
    void animate(MoveUnitAction& act, int id, GameMap* m) {
	qDebug() << "move";
	Unit* u = m->findUnit(id);
	SpritesPack* pack = Sprites[u->name];

    }

    void animate(FireUnitAction& act, QString attName, Invoker* inv) {
	//qDebug() << "act.attackerName = " << act.attackerName;
	qDebug() << "fire";
	SpritesPack* temp = Sprites[act.attackerName];
	curPack = dynamic_cast<UnitPack*>(temp);
	sprites = curPack->attack;
	_invoker = inv;
	animHelper->setEndValue(sprites.count()-1);
	aniGroup->removeAnimation(animHelperMove);
	//qDebug() << "\tstart animation";
	aniGroup->start();
    }
    int curSprite() {
	return _curSprite;
    }
    void setCurSprite(int _x) {
	_curSprite =  _x;
	const QPixmap p = sprites.at(_curSprite);
	setPixmap(p);
    }
signals:
    void animationEnded(Invoker*);
private slots:
    void propAnimEnded() {
	qDebug() << "\tend animation";

/*	FireUnitAction * act = dynamic_cast<FireUnitAction*>(_lastAct);
	if (act!=NULL) {
	    int id = act->victimID;

	} */
	emit animationEnded(_invoker);
    }
public slots:

};

#endif // GAMETEXTUREITEM_H
