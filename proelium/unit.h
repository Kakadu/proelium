#ifndef UNIT_H
#define UNIT_H
#include "framecollection.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QtCore>
#include <QTimer>

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

public:
    IDT id;
    QString name;

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
