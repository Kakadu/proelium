#include "gametextureitem.h"

GameTextureItem::GameTextureItem(QGraphicsScene* sc, int tsw, int tsh) :
    QGraphicsPixmapItem(NULL,sc), _terrSpriteWidth(tsw), _terrSpriteHeight(tsh)
{
    DURATION = 250;
    //aniGroup = new QParallelAnimationGroup(this);
}
