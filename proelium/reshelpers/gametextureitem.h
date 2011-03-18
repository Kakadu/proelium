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

class GameTextureItem : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
    Q_PROPERTY(int curSprite READ curSprite WRITE  setCurSprite);
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset);
    int DURATION;
    QPropertyAnimation* animHelperFire;
    QPropertyAnimation* animHelperMove;
    QPropertyAnimation* animHelperDeath;
    QAnimationGroup* aniGroup;
    uint _curSprite;
    UnitPack* curPack;
    QVector<QPixmap> sprites;

    AbstractUnitAction* _lastAct;
    int _terrSpriteWidth, _terrSpriteHeight;    

    QPropertyAnimation* initHelper(QByteArray prop) {
	QPropertyAnimation* ani = new QPropertyAnimation(this);
	ani->setDuration(DURATION);
	ani->setTargetObject(this);
	ani->setPropertyName(prop);
	ani->setStartValue(0);
	ani->setEndValue(sprites.count()-1);
	return ani;
    }

    void initFireHelper() {
	animHelperFire = initHelper("curSprite");
	QObject::connect(animHelperFire, SIGNAL(finished()),
			 this, SLOT(setDefaultSprite()));
    }
    void initDeathHelper() {
	animHelperDeath = initHelper("curSprite");
	QObject::connect(animHelperDeath, SIGNAL(finished()),
			 this, SLOT(setDefaultSprite()));
    }
    void initMoveHelper(int dx1, int dy1) {
	animHelperMove = new QPropertyAnimation(this);
	animHelperMove->setDuration(DURATION);
	animHelperMove->setTargetObject(this);
	animHelperMove->setPropertyName("offset");
	animHelperMove->setStartValue(this->offset());
	animHelperMove->setEndValue(QPointF(offset().x()+dx1,
					    offset().y()+dy1 ) );
	QObject::connect(animHelperMove, SIGNAL(finished()),
			 this, SLOT(setDefaultSprite()));
    }

public:
    explicit GameTextureItem(QGraphicsScene*,int,int);
    QAnimationGroup* animate(MoveUnitAction& act, int, GameMap* const , QPoint& newLoc) {
	Unit* u = act.unit();
	SpritesPack* pack = Sprites[u->name];
	curPack = dynamic_cast<UnitPack*>(pack);
	sprites = curPack->move;
	/*
	  Тут сложности с пересчетом координат. Проблема в том, что размеры картинок
	  разные. Т.е. чтобы перейти из спрайта в нормальном состоянии к движению,
	  надо подсчитать разницу в размере спрайтов, потом исправить offset, и потом
	  запустить анимацию. А в конце вернуть в обратное состояние.
	  */
	int dx1 = newLoc.x() - (offset().x() + pixmap().width()/2 - _terrSpriteWidth/2),
	    dy1 = newLoc.y() - (offset().y() + pixmap().height()/2 - _terrSpriteHeight/2);

	//setNewPixmap(sprites.at(0));

	aniGroup = new QParallelAnimationGroup(this);

	initFireHelper(); // rename or incapsulate move, fire and death in 1 helper
	initMoveHelper(dx1,dy1);
	aniGroup->addAnimation(animHelperFire);
	aniGroup->addAnimation(animHelperMove);
	return aniGroup;
    }
    QAnimationGroup* animate(FireUnitAction& act) {
	SpritesPack* temp = Sprites[act.attackerName];
	curPack = dynamic_cast<UnitPack*>(temp);
	sprites = curPack->attack;

	aniGroup = new QSequentialAnimationGroup(this);
	initFireHelper();
	aniGroup->addAnimation(animHelperFire);
	return aniGroup;
    }
    QAnimationGroup* animateDeath(QString unitName) {
	SpritesPack* temp = Sprites[unitName];
	curPack = dynamic_cast<UnitPack*>(temp);
	sprites = curPack->death;

	aniGroup = new QSequentialAnimationGroup(this);
	initDeathHelper();
	aniGroup->addAnimation(animHelperDeath);

	return aniGroup;
    }
    void setNewPixmap(const QPixmap& p) {
	//Q_ASSERT(!p.isNull());
	int dx = pixmap().width()/2  - p.width()/2,
	    dy = pixmap().height()/2  - p.height()/2;
	setPixmap(p);
	setOffset(offset().x()+dx, offset().y()+dy);
    }

    inline void setSpriteHelper(const int& x) {
	Q_ASSERT(sprites.count()>x);
	const QPixmap p = sprites.at(x);
	setNewPixmap(p);
    }
    inline int curSprite() {
	return _curSprite;
    }
    inline void setCurSprite(const int& x) {
	_curSprite = x;
	setSpriteHelper(x);
    }

    inline QAnimationGroup* animationGroup() {
	return aniGroup;
    }

signals:
    void animationEnded();

private slots:
    void propAnimEnded() {
	qDebug() << "\tend animation";
	emit animationEnded();
    }
public slots:
    void setDefaultSprite() {
	const QPixmap p = curPack->normal.at(0);
	setNewPixmap(p);
    }
    void setDeathSprite() {
	int len = curPack->death.count();
	const QPixmap p  = curPack->death.at(len-4);
	setNewPixmap(p);
    }
};

#endif // GAMETEXTUREITEM_H
