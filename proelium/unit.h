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

    explicit Unit(QString _name, int _id, QObject* parent = NULL) : QObject(parent) {
	name = _name;
	id = _id;
    }
    /*
    explicit Unit(Unit& u) {
	name = u.name;
	id = u.id;
    }
    */

/*
    void setAttackFrame(int n)  {
	_curframe = n;
	QPixmap* map = _attackFrames->getImage(n);
	_view->setPixmap(*map);
    }
    int attackFrame() {
	return _curframe;
    }

    void animate(int) {
	QPropertyAnimation *an1 = new QPropertyAnimation(this,"attackFrame");
	an1->setEndValue(_attackFrames->count() -1);
	an1->setStartValue(0);
	an1->setDuration(2000);
	an1->start();
    }
*/
};

#endif // UNIT_H
