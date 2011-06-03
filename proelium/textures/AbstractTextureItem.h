#ifndef ABSTRACTTEXTUREITEM_H
#define ABSTRACTTEXTUREITEM_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class AbstractTextureItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    AbstractTextureItem(QGraphicsScene *sc) : QGraphicsPixmapItem(NULL,sc) {

    }
    /*
    void setOffset(QPoint &p) {
        setOffset(p.x(),p.y());
    }
    */
    virtual ~AbstractTextureItem() {}
};
#endif // ABSTRACTTEXTUREITEM_H
