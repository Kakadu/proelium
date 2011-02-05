#ifndef UNIT_H
#define UNIT_H
#include "framecollection.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QtCore>
#include <QTimer>

class Unit : public QObject  {
    Q_OBJECT
    Q_PROPERTY(int attackFrame WRITE setAttackFrame READ attackFrame);
private:
    FrameCollection* _attackFrames;
    FrameCollection* _deathFrames;
    FrameCollection* _moveFrames[8];
    QGraphicsPixmapItem* _view;
    QTimer* _timer;
    int _curframe;

public:
    Unit(FrameCollection* att_col, QGraphicsPixmapItem* view)
	: _attackFrames(att_col), _view(view) {	
	_timer = new QTimer(NULL);
    }

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

    /*void nextFrame() {
	_curframe++;
	if (_curframe == _attackFrames->count() )
	    _curframe = 0;
    }*/
};

#endif // UNIT_H
