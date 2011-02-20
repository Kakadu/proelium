#ifndef GAMETEXTUREITEM_H
#define GAMETEXTUREITEM_H

#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "rescontainer.h"
#include <QDebug>

extern QMap<QString, SpritesPack*> Sprites;

class GameTextureItem : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
    Q_PROPERTY(int curSprite READ curSprite WRITE setCurSprite);
    QPropertyAnimation* animHelper;
    uint _curSprite;
    UnitPack* curPack;
public:
    explicit GameTextureItem(QGraphicsScene*);
    void animate(QString attName) {
	SpritesPack* temp = Sprites[attName];
	curPack = dynamic_cast<UnitPack*>(temp);
	animHelper->setStartValue(0);
	animHelper->setTargetObject(this);
	animHelper->setPropertyName("curSprite");
	animHelper->setEndValue(curPack->attack.count()-1);
	animHelper->start();
    }
    int curSprite() {
	return _curSprite;
    }
    void setCurSprite(int _x) {
	const QPixmap p = curPack->attack.at(_curSprite = _x);
	setPixmap(p);
    }
signals:
    void animationEnded();
private slots:
    void propAnimEnded() {
	qDebug() << "propAnimationFinished";
	emit animationEnded();
    }
public slots:

};

#endif // GAMETEXTUREITEM_H
