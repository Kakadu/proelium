#include "gametextureitem.h"

GameTextureItem::GameTextureItem(QGraphicsScene* sc ) :
    QGraphicsPixmapItem(NULL,sc)
{
    animHelper = new QPropertyAnimation(this);
    animHelper->setStartValue(0);
    animHelper->setDuration(2000);
    QObject::connect(animHelper, SIGNAL(finished()),
		     this,    SLOT(propAnimEnded()));
}
