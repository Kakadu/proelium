#ifndef MAINHELPER_H
#define MAINHELPER_H

#include <QObject>
#include "beforeWar.h"
#include "mainwindow.h"
#include "GameMap.h"
#include "mapdrawer.h"
#include "GlobalConst.h"
#include "fightingmodel.h"
#include "other_fighting_models.h"
#include "beforeWar.h"

class MainHelper : public QObject
{
Q_OBJECT
public:
    explicit MainHelper(QObject *parent = 0);
    ~MainHelper() {

    }

    BeforeWar* dialog;
    MainWindow* w;
    MapDrawer* drawer;
    MainFightingModel*    model1;
    ComplexFightingModel* model2;
    PtursFightingModel*   model3;
    ModelParam* param;
    GameMap *m;
signals:

public slots:
    void showMainWindow() {
	param = new ModelParam;

	Ui::BeforeWar  *ui = dialog->getUI();
        param->d30PlatoonCount = ui->d30Count->value()*6;
        param->tankPlatoonCount = ui->tanksCount->value()*18;
        param->N_tanks = param->tankPlatoonCount;
        param->pturPlatoonCount = ui->pturCount->value()*3;

	param->tankSurrender = ! ui->NeverSurrenderRadio->isChecked();
        if (param->tankSurrender)
            param->tankSurrenderAt = ui->surrenderSpin->value();
        else
            param->tankSurrenderAt = 0;


	if (ui->TDD1_radio->isChecked())
	    param->TON_type = 0;
	else if (ui->TDD2_radio->isChecked())
	    param->TON_type = 1;
	else
	    param->TON_type = 2;

        if (param->TON_type == 2)
            param->d30PlatoonCount=0;

        m = new GameMap(12,7);
	m->init();

	QGraphicsScene* sc = w->getScene();
	drawer  = new MapDrawer(sc,m);

	drawer->paintField();
        drawer->placeArmies(param);
	drawer->repaint();

        if (param->TON_type==1) {
            model1 = new MainFightingModel(m, param);
            w->show();
            QObject::connect(model1,SIGNAL(action(AbstractUnitAction*)),
                             drawer,SLOT(applyAction(AbstractUnitAction*)) );

            QObject::connect(drawer, SIGNAL(continueModel()),
                             model1, SLOT(next()) );

            model1->next();
        }
        else if (param->TON_type==0) {
            model2 = new ComplexFightingModel(m, param);
            w->show();
            QObject::connect(model2,SIGNAL(action(AbstractUnitAction*)),
                            drawer,SLOT(applyAction(AbstractUnitAction*)) );

            QObject::connect(drawer, SIGNAL(continueModel()),
                            model2, SLOT(next()) );

            model2->next();
    }   else if (param->TON_type==2) {
            model3 = new PtursFightingModel(m, param);
            w->show();
            QObject::connect(model3,SIGNAL(action(AbstractUnitAction*)),
                            drawer,SLOT(applyAction(AbstractUnitAction*)) );

            QObject::connect(drawer, SIGNAL(continueModel()),
                            model3, SLOT(next()) );

            model3->next();
    }
    }
};

#endif // MAINHELPER_H
