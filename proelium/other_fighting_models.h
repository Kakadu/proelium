#ifndef OTHER_FIGHTING_MODELS_H
#define OTHER_FIGHTING_MODELS_H
#include "fightingmodel.h"


// Война сначала с ПТУРами и после гаубицами
class ComplexFightingModel: public QObject, protected FightingModel {
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
    ComplexFightingModel(GameMap* m, ModelParam* current) {
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
            if ((defenders.count() == 0) || (tanks.count() == 0 /*<= model_descrip.get_N_refusal()*/)) {
                emit action(new EndWarAction());
                return;
            }

            if (!new_borns.empty()) {
                new_borns.clear();
                emit action(new NewUnitsAppearedAction());
                return;
            }

            if (shot_order==0)  {

                Unit* additional;
                for (int i = 0; i <=2; i++) {
                    additional = new Unit ("ptur", _map->nextId() );
                    _map->getSquare1((h+1)/2+i,s-(h+1)/2-i)->addUnit(additional);
                    pturs.push_back(additional);
                    new_borns.append(additional);
                }
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

            if (shot_order==2)  {
                qDebug()<<"shot_order = 2"<<endl;
                Unit* additional;
                for (int i=0; i <= w; i++)   {
                    additional = new Unit("tank",  _map->nextId() );
                    _map->getSquare1(i,w-i)->addUnit(additional);
                    tanks.push_back(additional);
                    new_borns.append(additional);

                    additional = new Unit("tank", _map->nextId() );
                    _map->getSquare1(i,w+1-i)->addUnit(additional);
                    tanks.push_back(additional);
                    new_borns.append(additional);
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
                succes = random()%4;
                succes = !succes;
                act = new FireUnitAction(attacker->id, attacker->name,
                                         victim->id, victim->name, succes);
                if (!(succes))
                    tanks.push_back(victim);
                emit action(act);
            }



            tank_queue++;

            qDebug()<<"We are here"<<endl;
        }
     }
};



#endif // OTHER_FIGHTING_MODELS_H
