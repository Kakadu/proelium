#ifndef ANIGROUP_H
#define ANIGROUP_H

#include <QAnimationGroup>
#include <QAbstractAnimation>
#include <QTimer>
#include "reshelpers/gametextureitem.h"

class RemoveTimer : public QTimer {
Q_OBJECT
    GameTextureItem* _item;
public:
    explicit RemoveTimer(QObject* parent, GameTextureItem* item)
	: QTimer(parent), _item(item) {
	QObject::connect(this, SIGNAL(timeout()),
			 this, SLOT(timeoutHandler()));
	setSingleShot(true);
	setInterval(1);
    }

signals:
    void timeout2(GameTextureItem*);

public slots:
    void timeoutHandler() {
//	QTimer::timeout();
	emit timeout2(_item);
    }
};

class AniGroup : public QObject {
Q_OBJECT
    QSequentialAnimationGroup* seq;
    QParallelAnimationGroup* par;
    RemoveTimer* _timer;
public:
    explicit AniGroup(QObject *parent = 0);
    void clear() {
	_timer = NULL;
	seq->clear();
	par->clear();
    }
    void addAnimationSeq(QAbstractAnimation* a) {
	seq->addAnimation(a);
    }
    void addAnimationPar(QAbstractAnimation* a) {
	par->addAnimation(a);
    }
signals:
    void finished();
private slots:
    void aniEnded() {
	if (_timer!=NULL) {
	    _timer->start();
	    seq->disconnect(_timer);
	    par->disconnect(_timer);
	}
	emit finished();
    }

public slots:
    void startSeq(RemoveTimer* timer) {
	_timer = timer;
	seq->start();
    }
    void startPar(RemoveTimer* timer) {
	_timer = timer;
	par->start();
    }
};

#endif // ANIGROUP_H
