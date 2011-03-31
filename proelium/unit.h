#ifndef UNIT_H
#define UNIT_H
#include "framecollection.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QtCore>
#include <QTimer>
#include <iostream>

typedef int IDT;

class Unit : public QObject  {
    Q_OBJECT
    //Q_PROPERTY(int attackFrame WRITE setAttackFrame READ attackFrame);
private:
    FrameCollection* _attackFrames;
    FrameCollection* _deathFrames;
    FrameCollection* _moveFrames[8];
    QGraphicsPixmapItem* _view;
    QTimer* _timer;
    int _curframe,;
    int _allPath;
    int _nonEvaledPath;
    bool is_alive;
public:
    IDT id;
    QString name;
    inline int allPath() { return _allPath; }
    inline int nonEvaledPath() { return _nonEvaledPath; }
    inline void incrPath(int x) {
	_allPath+=x; _nonEvaledPath+=x;
    }
    inline void afterEvalPath(int x) {
	if (x>_nonEvaledPath)
	    throw std::exception();
	_nonEvaledPath -=x;
    }

    Unit& operator=(const Unit& u) {
	Unit* a = new Unit(u.name, u.id);
	return *a;
    }
    bool alive (void) {return is_alive;}
    void setAlive(bool b) {
        is_alive = b;
    }

    explicit Unit(QString _name, int _id, QObject* parent = NULL) : QObject(parent) {
	name = _name;
	id = _id;
        is_alive = true;
    }

};

#endif // UNIT_H
