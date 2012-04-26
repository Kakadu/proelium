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

template<typename T1, typename T2, typename T3>
class Triple {
    T1 _a;
    T2 _b;
    T3 _c;
public:
    Triple(T1 a, T2 b, T3 c) : _a(a), _b(b), _c(c) {}
    T1 a() { return _a; }
    T1 b() { return _b; }
    T1 c() { return _c; }
    Triple(const Triple<T1,T2,T3> &cpy) {
        _a = cpy.a(); _b = cpy.b(); _c = cpy.c();
    }
};

#endif // ABSTRACTIONS_H
