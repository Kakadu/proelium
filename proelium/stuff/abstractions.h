#ifndef ABSTRACTIONS_H
#define ABSTRACTIONS_H

#include <QDebug>

class AbstractDrawer
{
    //Q_OBJECT
public:
     void repaint() {
        qDebug() << "AbstractDrawer::repaint";
     }
};

#endif // ABSTRACTIONS_H
