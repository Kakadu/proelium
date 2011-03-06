#include "gametextureitem.h"

GameTextureItem::GameTextureItem(QGraphicsScene* sc ) :
    QGraphicsPixmapItem(NULL,sc)
{
    DURATION = 2000;
    animHelper = new QPropertyAnimation(this);
    animHelper->setStartValue(0);
    animHelper->setDuration(DURATION);
    animHelper->setTargetObject(this);
    animHelper->setPropertyName("curSprite");
    animHelper->setStartValue(0);

    animHelperMove = new QPropertyAnimation(this);
    animHelperMove->setStartValue(0);
    animHelperMove->setDuration(DURATION);

    aniGroup = new QParallelAnimationGroup(this);
    aniGroup->addAnimation(animHelper);
    aniGroup->addAnimation(animHelperMove);

    QObject::connect(aniGroup, SIGNAL(finished()),
		     this,    SLOT(propAnimEnded()));
}
