#ifndef TERRAINTEXTUREITEM_H
#define TERRAINTEXTUREITEM_H
#include "textures/AbstractTextureItem.h"

class TerrainTextureItem : public AbstractTextureItem {
public:
    TerrainTextureItem(QGraphicsScene *sc) : AbstractTextureItem(sc) {

    }
    void mousePressEvent(QGraphicsSceneMouseEvent *) {
        qDebug() << "terrain clicked";
    }
};

#endif // TERRAINTEXTUREITEM_H
