#ifndef FIGHTINGMODEL_H
#define FIGHTINGMODEL_H
#include "GameMap.h"
#include "action/action.h"
#include <QVector>
#include <QQueue>
#include "reshelpers/rescontainer.h"

class FightingModel {
protected:
    GameMap* _map;
public:
    FightingModel() {}
};
class SimpleFightingModel: public QObject, protected FightingModel {
    Q_OBJECT
private:
    bool ended;
    QQueue<Unit*> tanks;
    QQueue<Unit*> defenders;
public:
    SimpleFightingModel(GameMap* m) {
	this->_map = m;
	ended = false;
	int s = m->width() + m->height() + 2;
	MapSquare* sq;
	for (int i=0; i<s; ++i)
	    for (int j=0; j<s; ++j) {
		sq = m->getSquare1(i,j);
		if (sq==NULL)
		    continue;
		foreach (Unit* u, sq->units) {
		    if (u->id > 100)
			defenders.push_back(u);
		    else
			tanks.push_back(u);
		}
	}
    }
signals:
    void action(AbstractUnitAction*);

public slots:
    void next() {
	qDebug() << "model.next";
	if ((defenders.count()==0) || (tanks.count() == 0)) {
	    emit action(new EndWarAction());
	    return;
	}
	Unit* mainTank = tanks.dequeue();
	if (random()%4==0) {
	    // tank fires at the random unit
	    Unit* victim   = defenders.dequeue();
	    tanks.push_back(mainTank);
	    FireUnitAction* act = new FireUnitAction(mainTank->id, mainTank->name,
						     victim->id, victim->name, true);
	    emit action(act);
	} else {
	    int i,j;
	    MapSquare* initSq = _map->locateUnit(i,j,mainTank);
	    MoveUnitAction* act;
	    MapSquare* sq;
	    if ((sq = _map->getSquare1(i+1,j)) != NULL) {
		act = new MoveUnitAction(i+1,j,mainTank);
		initSq->removeUnit(mainTank);
		sq->addUnit(mainTank);
		mainTank->incrPath(45);
	    } else if  ((sq = _map->getSquare1(i,j+1)) != NULL) {
		act = new MoveUnitAction(i,j+1,mainTank);
		initSq->removeUnit(mainTank);
		sq->addUnit(mainTank);
		mainTank->incrPath(45);
	    }
	    tanks.push_back(mainTank);
	    emit action(act);
	}
    }
};

#endif // FIGHTINGMODEL_H
