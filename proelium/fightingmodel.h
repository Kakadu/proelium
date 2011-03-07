#ifndef FIGHTINGMODEL_H
#define FIGHTINGMODEL_H
#include "GameMap.h"
#include "action/action.h"
#include <QVector>
#include <QQueue>
#include "reshelpers/rescontainer.h"

/**
  Базовый класс моделей боя. Наследуйте его для своих моделей.
  */
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
    /**
      Вызывем этот сигнал, когда мы высчитали следующее действие. На этот сигнал
      подписывается отрисовщик.
      */
    void action(AbstractUnitAction*);

public slots:
    /**
      Здесь вычисляется следующее действие битвы. Доступ к юнитам осуществляется
      через две очереди. Выдрать нужную информацию из карты можно в конструкторе,
      но в принципе можно хранить обрашаться к мапе используя this->_map.
      Разруливаться в каком порядке юниты должны стрелять надо внутри этого класса.
      Когда юнит должен двигаться, а когда стрелять - тоже тут. Сколько юнит проехал
      и надо ли его передвинуть на следующую клетку - тоже тут.
    */
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
	    // TODO: надо нормально написать в каких направлениях юнит будет двигаться
	    // к цели. думаю направлений будет всего три: Юг, Юго-Запад, и Юго-Восток.
	    if ((sq = _map->getSquare1(i+1,j)) != NULL) {
		act = new MoveUnitAction(i,j,i+1,j,mainTank);
		initSq->removeUnit(mainTank);
		sq->addUnit(mainTank);
		//mainTank->incrPath(45);
	    } else if  ((sq = _map->getSquare1(i,j-1)) != NULL) {
		act = new MoveUnitAction(i,j,i,j-1,mainTank);
		initSq->removeUnit(mainTank);
		sq->addUnit(mainTank);
		//mainTank->incrPath(45);
	    }
	    tanks.push_back(mainTank);
	    qDebug() << act;
	    emit action(act);
	}
    }
};

#endif // FIGHTINGMODEL_H
