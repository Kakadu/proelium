#include "gametextureitem.h"

GameTextureItem::GameTextureItem(QGraphicsScene* sc, int tsw, int tsh) :
    QGraphicsPixmapItem(NULL,sc), _terrSpriteWidth(tsw), _terrSpriteHeight(tsh)
{
    DURATION = 2000;
    animHelper = new QPropertyAnimation(this);
    animHelper->setStartValue(0);
    animHelper->setDuration(DURATION);
    animHelper->setTargetObject(this);
    animHelper->setPropertyName("curSprite");    

    animHelperMove = new QPropertyAnimation(this);
    animHelperMove->setDuration(DURATION);
    animHelperMove->setTargetObject(this);
    animHelperMove->setPropertyName("offset");


    aniGroup = new QParallelAnimationGroup(this);
    aniGroup->addAnimation(animHelper);
    aniGroup->addAnimation(animHelperMove);

    QObject::connect(aniGroup, SIGNAL(finished()),
		     this,    SLOT(propAnimEnded()));
}
