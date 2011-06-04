#ifndef TERRAINTEXTUREITEM_H
#define TERRAINTEXTUREITEM_H

#include <QPropertyAnimation>
#include "textures/AbstractTextureItem.h"
#include "reshelpers/rescontainer.h"

extern QMap<QString, SpritesPack*> Sprites;

class TerrainTextureItem : public AbstractTextureItem
{
    QString _resourceName;
    QPropertyAnimation* animator;
    int DURATION;
    TerrainPack* sprites;
    int _curSprite;
public:
    Q_PROPERTY(int sprite READ curSprite WRITE  setCurSprite);
    TerrainTextureItem(QGraphicsScene *sc, const QString &resName)
        : AbstractTextureItem(sc), _resourceName(resName) {
        DURATION = 250;
        sprites = dynamic_cast<TerrainPack*>(Sprites[_resourceName]);
        if (sprites == NULL)
            throw "cannot eval current sprites";
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *) {
        qDebug() << "terrain clicked";
    }
    int curSprite() { return _curSprite; }
    void setCurSprite(int x) {
        Q_ASSERT(x>=0 && x<sprites->content.count());
        _curSprite = x;
        setPixmap(sprites->content[_curSprite]);
    }

    void animate() {
        animator = new QPropertyAnimation(this);
        animator->setDuration(DURATION);
        animator->setTargetObject((TerrainTextureItem*)this);
        animator->setPropertyName("sprite");
        animator->setStartValue(0);
        animator->setEndValue(sprites->content.count()-1);
        QObject::connect(animator,SIGNAL(finished()),
                         (TerrainTextureItem*)this, SLOT(animateRestart()) );
    }
public slots:
    void animateRestart() {
        if (animator == NULL)
            return;
        animator->start();
    }
};

#endif // TERRAINTEXTUREITEM_H
