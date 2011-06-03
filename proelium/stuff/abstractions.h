#ifndef ABSTRACTIONS_H
#define ABSTRACTIONS_H

#include <QDebug>
#include <QGraphicsPixmapItem>

class AbstractDrawer
{
public:
     virtual void repaint() {
        qDebug() << "AbstractDrawer::repaint\nIf u see this message You'll find bug in code";
     }
    virtual void showCursorSprite(const int&,const int&)=0;
};

#endif // ABSTRACTIONS_H
