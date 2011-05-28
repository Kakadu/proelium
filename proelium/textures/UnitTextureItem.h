#ifndef UNITTEXTUREITEM_H
#define UNITTEXTUREITEM_H

#include <QGraphicsScene>
#include <qgraphicsscene.h>
#include <QGraphicsSceneMouseEvent>
#include "textures/AbstractTextureItem.h"
#include "action/UserActionHypervisor.h"

extern  UserActionHyperVisor* MainHyperVisor;

// An idea to separate textures for terrain and textures for units
// because them have different mouse handlers
class UnitTextureItem : public AbstractTextureItem {
private:
    Unit* _unit;
public:
    UnitTextureItem(QGraphicsScene *sc, Unit* u)
        : AbstractTextureItem(sc), _unit(u) {

    }
    Unit* getUnit() {
        return _unit;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        qDebug() << "mouse pressed " << event->buttonDownPos(Qt::LeftButton);
        if (MainHyperVisor == nullptr)
            return;
        MainHyperVisor->setCurUnit(_unit);

    }
};

#endif // UNITTEXTUREITEM_H
