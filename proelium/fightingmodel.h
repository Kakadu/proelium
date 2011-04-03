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
    Unit* unit2add;
public:
    SimpleFightingModel(GameMap* m, ModelParam *) {
        unit2add=
                NULL;
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
    /**7
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
	    emit action(new EndWarAction(""));
	    return;
	}

        if (unit2add!=NULL) {
            unit2add=NULL;
            emit action(new NewUnitsAppearedAction());
            return;
        }
	Unit* mainTank = tanks.dequeue();

        if (qrand()%4==0) {
	    // tank fires at the random unit
	    Unit* victim   = defenders.dequeue();
	    tanks.push_back(mainTank);
	    FireUnitAction* act = new FireUnitAction(mainTank->id, mainTank->name,
						     victim->id, victim->name, true);
            victim->setAlive(false);
            int i,j;
            _map->locateUnit(i,j,victim);

            unit2add = new Unit ("tank", 7000+qrand()%100);
            _map->getSquare1(i-4,j-4)->addUnit(unit2add);
            tanks.push_back(unit2add);
	    emit action(act);
	} else {
	    int i,j;
	    MapSquare* initSq = _map->locateUnit(i,j,mainTank);
	    MoveUnitAction* act;
	    MapSquare* sq;
	    // TODO: надо нормально написать в каких направлениях юнит будет двигаться
	    // к цели. думаю направлений будет всего три: Юг, Юго-Запад, и Юго-Восток.
	    QPair<int,int> dd(0,0);
            int d = 0;
	    while (true) {
//		int d = random()%3;
//                int d = 0;
                dd = directions[d];
		sq = _map->getSquare1(i+dd.first,j+dd.second);
		if (sq!=NULL)
		    break;
                d++;
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
	    emit action(new EndWarAction(""));
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
    int shot_order;
    int possib;
    bool succes;
    QQueue<Unit*> tanks;
    QQueue<Unit*> defenders;
    QQueue<Unit*> pturs;
    QList <Unit*> new_borns;
    ModelParam model_descrip;
public:
    MainFightingModel(GameMap* m, ModelParam* current)   {
        this->_map = m;
        model_descrip = *current;
        possib = 5;
        tank_queue = 0;
        shot_order = 0;
        new_borns.clear();
        int s = m->width() + m->height() + 2;
        MapSquare* sd;
        for (int i = 0; i < s; ++i)
            for (int j = 0; j < s; ++j) {
            sd = m->getSquare1(i,j);

            if (sd==NULL)
                continue;
            foreach(Unit* u, sd->units) {
                if (u->name.compare(QString("tank")) != 0 )
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
//            if ((defenders.count() == 0) || (tanks.count() == 0 /*<= model_descrip.get_N_refusal()*/)) {
//		emit action(new EndWarAction(""));
//              return;
//        }

            if ((defenders.count() == 0) || ((tanks.count() <= model_descrip.N_tanks*(100-model_descrip.tankSurrenderAt)/100) && (shot_order>3))) {
                if (!defenders.count())
                    emit action (new EndWarAction("Cannons are loosers"));
                else if (tanks.count()>0)
                    emit action(new EndWarAction("Tanks stepped back"));
                else
                    emit action(new EndWarAction("Tanks are loosers"));
                return;
            }

            if (!new_borns.empty()) {
                new_borns.clear();
                emit action(new NewUnitsAppearedAction());
                return;
            }

            if (shot_order==1)  {
                qDebug()<<"shot_order = 2"<<endl;
                int w  = _map->width();
                Unit* additional;
                for (int i=0; i <= w; i++)   {
                    if (i<model_descrip.tankPlatoonCount/2) {
                        additional = new Unit("tank",i+w+qrand()%1000);
                        _map->getSquare1(i,w-i)->addUnit(additional);
                        tanks.push_back(additional);
                        new_borns.append(additional);

                        additional = new Unit("tank",i+w*2+qrand()%1000);
                        _map->getSquare1(i,w+1-i)->addUnit(additional);
                        tanks.push_back(additional);
                        new_borns.append(additional);
                    }
                }
                qDebug()<<"shot_order = 2"<<endl;
                shot_order++;
                emit action (new NoAction());
               return;
            }
            else {
//            FireUnitAction* act = NULL;
	    if (!(tank_queue%possib))    {
                  if (shot_order<=5)       {
                      possib=5;
                  for (int shet=0; shet<tanks.count(); shet++)  {

  /*              if (shot_order==1)   {
                    int w  = _map->width();
                    int h  = _map->height();
        //            for (int jr=0; jr<=w; ++jr) {
                        Unit* unit = new Unit("tank",0+w*2);
                        _map->getSquare1(0,w)->addUnit(unit);
                        emit (new NewUnitsAppearedAction());
                        shot_order++;
                        return;
         //           }
                }*/
                Unit* mainTank = tanks.dequeue();
                int i,j;
                MapSquare* initSq = _map->locateUnit(i,j,mainTank);
//                qDebug()<<i<<"\t"<<j<<endl;
                MoveUnitAction* act;
                MapSquare* sq;
                QPair<int,int> dd(0,0);

                    dd = directions[1];
                    sq = _map->getSquare1(i+dd.first,j+dd.second);
//                qDebug()<<dd.first<<"\t"<<dd.second<<endl;

                    act = new MoveUnitAction(i,j,i+dd.first,j+dd.second, mainTank);
//                qDebug()<<"We are here"<<endl;
                    initSq->removeUnit(mainTank);
                    sq->addUnit(mainTank);
//                qDebug()<<"We are here"<<endl;
                tanks.push_back(mainTank);
//                qDebug()<<"We are here"<<endl;
                emit action(act);
            }
              shot_order++;
          }   else {
              possib=3;
                FireUnitAction* act = NULL;
                Unit* mainTank = tanks.dequeue();
                Unit* victim   = defenders.dequeue();
                tanks.push_back(mainTank);
                succes = qrand()%5;
                succes = !succes;
                act = new FireUnitAction(mainTank->id, mainTank->name,
                                         victim->id, victim->name, succes);
                if (!succes)
                    defenders.push_back(victim);
                emit action(act);
            }
                }
            else    {
                FireUnitAction* act = NULL;
                Unit* attacker = defenders.dequeue();
                Unit* victim   = tanks.dequeue();
                defenders.push_back(attacker);
                succes = qrand()%4;
                succes = !succes;
                act = new FireUnitAction(attacker->id, attacker->name,
                                         victim->id, victim->name, succes);
                if (!(succes))
                    tanks.push_back(victim);
                emit action(act);   }



            tank_queue++;

            qDebug()<<"We are here"<<endl;
        }
     }
};



// Война сначала с ПТУРами и после гаубицами
class PtursFightingModel: public QObject, protected FightingModel {
    Q_OBJECT
private:
    int tank_queue;
    int ptur_queue;
    int shot_order;
    int possib;
    bool succes;
    int w, h, s;
    QQueue<Unit*> tanks;
    QQueue<Unit*> defenders;
    QQueue<Unit*> pturs;
    QList <Unit*> new_borns;
    ModelParam model_descrip;
public:
    PtursFightingModel(GameMap* m, ModelParam* current) {
        _map = m;
        w = m->width();
        h = m->height();
        s = w + h + 2;
         model_descrip = *current;
        possib = 5;
        tank_queue = 0;
        ptur_queue = 0;
        shot_order = 0;
        new_borns.clear();
        int s = m->width() + m->height() + 2;
        MapSquare* sd;
        for (int i = 0; i < s; ++i)
            for (int j = 0; j < s; ++j) {
            sd = m->getSquare1(i,j);
            if (sd==NULL)
                continue;
            foreach(Unit* u, sd->units) {
                if (u->name.compare(QString("tank")) != 0 )
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
            qDebug()<<"ComplexFightingModel.next";
            //Раскоментить следующию строчку когда танков будет адекватное количество а не 8 штук
 //           if ((shot_order!=0 && (pturs.count() == 0)) || (tanks.count() == 0 /*<= model_descrip.get_N_refusal()*/)) {
 //               emit action(new EndWarAction("this is the end."));
 //               return;
 //           }

            if (((tanks.count() <= model_descrip.N_tanks*(100-model_descrip.tankSurrenderAt)/100)  && (shot_order>2)) || (pturs.count()==0 && shot_order>2)) {
                if (tanks.count()>0 && pturs.count())
                    emit action(new EndWarAction("Tanks stepped back"));
                else if (!pturs.count())
                    emit action(new EndWarAction("Pturs are looser"));
                else
                    emit action(new EndWarAction("Tanks are loosers"));
                return;
            }

            if (!new_borns.empty()) {
                new_borns.clear();
                emit action(new NewUnitsAppearedAction());
                return;
            }

            if (shot_order==0)  {

                Unit* additional;
                for (int i = w/2; i <=w/2+model_descrip.pturPlatoonCount-1; i++) {
                    additional = new Unit ("ptur", _map->nextId() );
                    _map->getSquare1((h+1)/2+i,s-(h+1)/2-i)->addUnit(additional);
                    pturs.push_back(additional);
                    new_borns.append(additional);
                }
                shot_order++;
                emit action (new NoAction());
                return;
            }

            if (shot_order==1) {
                for (int shet=0; shet<tanks.count(); shet++)  {

              Unit* mainTank = tanks.dequeue();
              int i,j;
              MapSquare* initSq = _map->locateUnit(i,j,mainTank);
              MoveUnitAction* act;
              MapSquare* sq;
              QPair<int,int> dd(0,0);

                  dd = directions[1];
                  sq = _map->getSquare1(i+dd.first,j+dd.second);
//                qDebug()<<dd.first<<"\t"<<dd.second<<endl;

                  act = new MoveUnitAction(i,j,i+dd.first,j+dd.second, mainTank);
//                qDebug()<<"We are here"<<endl;
                  initSq->removeUnit(mainTank);
                  sq->addUnit(mainTank);
//                qDebug()<<"We are here"<<endl;
              tanks.push_back(mainTank);
//                qDebug()<<"We are here"<<endl;
              emit action(act);

            }
            shot_order++;
            return;
            }

            if (shot_order==2)  {
                qDebug()<<"shot_order = 2"<<endl;
                Unit* additional;
                for (int i=0; i <= w; i++)   {
                    if (i<model_descrip.tankPlatoonCount/2) {
                        additional = new Unit("tank",  _map->nextId() );
                        _map->getSquare1(i,w-i)->addUnit(additional);
                        tanks.push_back(additional);
                        new_borns.append(additional);

                        additional = new Unit("tank", _map->nextId() );
                        _map->getSquare1(i,w+1-i)->addUnit(additional);
                        tanks.push_back(additional);
                        new_borns.append(additional);
                    }
                }
                qDebug()<<"shot_order = 2"<<endl;
                shot_order++;
                emit action (new NoAction());
               return;
            }

            if (pturs.count()!=0)   {
                FireUnitAction*  act = NULL;
                if (ptur_queue%2)   {
                    Unit* attaker = tanks.dequeue();
                    Unit* victim  = pturs.dequeue();
                    succes = qrand()%4;
                    succes = !succes;
                    act = new FireUnitAction(attaker->id, attaker->name,
                                             victim->id, victim->name, succes);
                    if (!succes)
                        pturs.push_back(victim);
                    else
                        victim->setAlive(false);
                    tanks.push_back(attaker);
                    ptur_queue++;
                    emit action(act);
                    return;
                } else {
                    Unit* attaker = pturs.dequeue();
                    Unit* victim  = tanks.dequeue();
                    succes = qrand()%20;
                    act = new FireUnitAction(attaker->id, attaker->name,
                                             victim->id, victim->name, succes);
                    if (!succes)
                        tanks.push_back(victim);
                    else
                        victim->setAlive(false);

                    pturs.push_back(attaker);

                    ptur_queue++;
                    emit action(act);
                    return;
                }

            }



return;


     }
};

#endif // FIGHTINGMODEL_H
