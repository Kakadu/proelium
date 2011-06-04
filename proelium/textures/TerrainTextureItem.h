#ifndef TERRAINTEXTUREITEM_H
#define TERRAINTEXTUREITEM_H

#include <QPropertyAnimation>
#include <QDebug>
#include "textures/AbstractTextureItem.h"
#include "reshelpers/rescontainer.h"

extern QMap<QString, SpritesPack*> Sprites;

class TerrainTextureItem : public AbstractTextureItem
{
    Q_OBJECT
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
    void setDuration(int x) {
        Q_ASSERT(x>0);
        DURATION = x;
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
public slots:
    void animateRestart() {
        if (animator == NULL)
            return;
        animator->start();
    }
public:
    void initAnimation() {
        animator = new QPropertyAnimation(this);
        animator->setDuration(DURATION);
        animator->setTargetObject(this);
        animator->setPropertyName("sprite");
        animator->setStartValue(0);
        animator->setEndValue(sprites->content.count()-1);
        QObject::connect(animator,SIGNAL(finished()),
                         this, SLOT(animateRestart()) );
        animator->start();
    }
    void startAnimation() {
        if (animator != NULL) {
            animator->start();
        } else {
            qDebug() << "animator is not initialized. forcing.";
            initAnimation();
            startAnimation();
        }
    }
};

#endif // TERRAINTEXTUREITEM_H
