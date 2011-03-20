#ifndef FIGHTINGMODEL_H
#define FIGHTINGMODEL_H
#include "GameMap.h"
#include "action/action.h"
#include <QVector>
#include <QQueue>
#include "reshelpers/rescontainer.h"
#include "GlobalConst.h"


extern QPair<int,int> directions[8];
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
	    QPair<int,int> dd(0,0);
	    while (true) {
//		int d = random()%3;
                int d = 0;
                dd = directions[d];
		sq = _map->getSquare1(i+dd.first,j+dd.second);
		if (sq!=NULL)
		    break;
	    }
	    if (sq == NULL) {
		throw "a very big problem in fighting model";
	    } else {
		act = new MoveUnitAction(i,j,i+dd.first,j+dd.second, mainTank);
		initSq->removeUnit(mainTank);
		sq->addUnit(mainTank);
	    }
	    tanks.push_back(mainTank);
	    emit action(act);
	}
    }
};

/**
 * Модель: ничего не двигается. стреляют обе стороны
 */
class SimpleFightingModel2: public QObject, protected FightingModel {
    Q_OBJECT
private:
    bool ended, tankIsNext;
    QQueue<Unit*> tanks;
    QQueue<Unit*> defenders;
public:
    SimpleFightingModel2(GameMap* m) {
	this->_map = m;
	ended = false;
	tankIsNext = true;
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
	qDebug() << "model2.next";
	if ((defenders.count()==0) || (tanks.count() == 0)) {
	    emit action(new EndWarAction());
	    return;
	}
	FireUnitAction* act = NULL;
	if (tankIsNext) {
	    Unit* mainTank = tanks.dequeue();
	    Unit* victim   = defenders.dequeue();
	    tanks.push_back(mainTank);
	    act = new FireUnitAction(mainTank->id, mainTank->name,
				     victim->id, victim->name, true);
	} else {
	    Unit* attacker = defenders.dequeue();
	    Unit* victim   = tanks.dequeue();
	    defenders.push_back(attacker);
	    act = new FireUnitAction(attacker->id, attacker->name,
				     victim->id, victim->name, true);
	}
	tankIsNext = !tankIsNext;
	emit action(act);
    }
};

class MainFightingModel: public QObject, protected FightingModel {
    Q_OBJECT
private:
    int tank_queue;
    bool succes;
    QQueue<Unit*> tanks;
    QQueue<Unit*> defenders;
    ModelParam model_descrip;
public:
    MainFightingModel(GameMap* m, ModelParam* current)   {
        this->_map = m;
        model_descrip = *current;
        tank_queue = 0;
        int s = m->width() + m->height() + 2;
        MapSquare* sd;
        for (int i = 0; i < s; ++i)
            for (int j = 0; j < s; ++j) {
            sd = m->getSquare1(i,j);
            if (sd==NULL)
                continue;
            foreach(Unit* u, sd->units) {
                if (u->id>100)
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
            qDebug()<<"MainModel.next";
            //Раскоментить следующию строчку когда танков будет адекватное количество а не 8 штук
            if ((defenders.count() == 0) || (tanks.count() == 0 /*<= model_descrip.get_N_refusal()*/)) {
                emit action(new EndWarAction());
                return;
            }
            FireUnitAction* act = NULL;
            if (!(tank_queue%3))    {
                Unit* mainTank = tanks.dequeue();
                Unit* victim   = defenders.dequeue();
                tanks.push_back(mainTank);
                succes = random()%5;
                succes = !(succes);
                act = new FireUnitAction(mainTank->id, mainTank->name,
                                         victim->id, victim->name, succes);
            }
            else    {
                Unit* attacker = defenders.dequeue();
                Unit* victim   = tanks.dequeue();
                defenders.push_back(attacker);
                succes = random()%3;
                succes = !(succes);
                act = new FireUnitAction(attacker->id, attacker->name,
                                         victim->id, victim->name, succes);

            }
            tank_queue++;
            emit action(act);
        }
};

#endif // FIGHTINGMODEL_H
