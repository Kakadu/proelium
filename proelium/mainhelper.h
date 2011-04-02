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
    ComplexFightingModel* model;
    ModelParam* param;
    GameMap *m;
signals:

public slots:
    void showMainWindow() {
	param = new ModelParam;

	Ui::BeforeWar  *ui = dialog->getUI();
	param->d30PlatoonCount = ui->d30Count->value();
	param->tankPlatoonCount = ui->tanksCount->value();
	param->pturPlatoonCount = ui->pturCount->value();

	param->tankSurrender = ! ui->NeverSurrenderRadio->isChecked();
	param->tankSurrenderAt = ui->surrenderSpin->value();

	if (ui->TDD1_radio->isChecked())
	    param->TON_type = 0;
	else if (ui->TDD2_radio->isChecked())
	    param->TON_type = 1;
	else
	    param->TON_type = 2;

	m = new GameMap(11,11);
	m->init();

	QGraphicsScene* sc = w->getScene();
	drawer  = new MapDrawer(sc,m);

	drawer->paintField();
	drawer->placeArmies();
	drawer->repaint();

        model = new ComplexFightingModel(m, param);
	w->show();
	QObject::connect(model,SIGNAL(action(AbstractUnitAction*)),
			 drawer,SLOT(applyAction(AbstractUnitAction*)) );

	QObject::connect(drawer, SIGNAL(continueModel()),
			 model, SLOT(next()) );

	model->next();
    }
};

#endif // MAINHELPER_H
