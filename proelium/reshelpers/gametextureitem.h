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

/*
enum AnimationType {
    ATTACK, MOVE, DEATH
};*/

class GameTextureItem : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
    Q_PROPERTY(int curSprite READ curSprite WRITE setCurSprite);
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset);
    int DURATION;
    QPropertyAnimation* animHelper;
    QPropertyAnimation* animHelperMove;
    QParallelAnimationGroup* aniGroup;
    uint _curSprite;
    UnitPack* curPack;
    QVector<QPixmap> sprites;

    AbstractUnitAction* _lastAct;
    int _terrSpriteWidth, _terrSpriteHeight;    

public:
    explicit GameTextureItem(QGraphicsScene*,int,int);
    void animate(MoveUnitAction& act, int, GameMap* const , QPoint& newLoc) {
	qDebug() << "move";
	Unit* u = act.unit();

	SpritesPack* pack = Sprites[u->name];
	curPack = dynamic_cast<UnitPack*>(pack);

	aniGroup->removeAnimation(animHelper);
	aniGroup->removeAnimation(animHelperMove);
	aniGroup->addAnimation(animHelperMove);
	sprites = curPack->move;
	/*
	  Тут сложности с пересчетом координат. Проблема в том, что размеры картинок
	  разные. Т.е. чтобы перейти из спрайта в нормальном состоянии к движению,
	  надо подсчитать разницу в размере спрайтов, потом исправить offset, и потом
	  запустить анимацию. А в конце вернуть в обратное состояние.
	  */
	int dx1 = newLoc.x() - (offset().x() + pixmap().width()/2 - _terrSpriteWidth/2),
	    dy1 = newLoc.y() - (offset().y() + pixmap().height()/2 - _terrSpriteHeight/2);

	setNewPixmap(sprites.at(0));
	animHelperMove->setStartValue(this->offset());

	animHelperMove->setEndValue(QPointF(offset().x()+dx1,
					    offset().y()+dy1 ) );

	aniGroup->start();
    }
    void setNewPixmap(const QPixmap& p) {
	int dx = pixmap().width()/2  - p.width()/2,
	    dy = pixmap().height()/2  - p.height()/2;
	setPixmap(p);
	setOffset(offset().x()+dx, offset().y()+dy);
    }

    void animate(FireUnitAction& act) {
	//qDebug() << "act.attackerName = " << act.attackerName;
	qDebug() << "fire";
	SpritesPack* temp = Sprites[act.attackerName];
	curPack = dynamic_cast<UnitPack*>(temp);
	sprites = curPack->attack;

	animHelper->setStartValue(0);
	animHelper->setEndValue(sprites.count()-1);

	aniGroup->removeAnimation(animHelper);
	aniGroup->removeAnimation(animHelperMove);
	aniGroup->addAnimation(animHelper);
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
    void animationEnded();

private slots:
    void propAnimEnded() {
	qDebug() << "\tend animation";
	const QPixmap p = curPack->normal.at(0);
	setNewPixmap(p);
	emit animationEnded();
    }
public slots:

};

#endif // GAMETEXTUREITEM_H
