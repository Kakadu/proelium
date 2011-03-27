#ifndef MAINHELPER_H
#define MAINHELPER_H

#include <QObject>
#include "beforeWar.h"
#include "mainwindow.h"
#include "GameMap.h"
#include "mapdrawer.h"
#include "GlobalConst.h"
#include "fightingmodel.h"

class MainHelper : public QObject
{
Q_OBJECT
public:
    explicit MainHelper(QObject *parent = 0);
    ~MainHelper() {
	//delete m;
	//delete drawer;
	//delete param;
    }

    BeforeWar* dialog;
    MainWindow* w;
    MapDrawer* drawer;
    MainFightingModel* model;
    ModelParam* param;
    GameMap *m;
signals:

public slots:
    void showMainWindow() {
	m = new GameMap(11,7);
	m->init();

	QGraphicsScene* sc = w->getScene();
	drawer  = new MapDrawer(sc,m);
	param = new ModelParam;

	drawer->paintField();
	drawer->placeArmies();
	drawer->repaint();

	model = new MainFightingModel(m, param);
	w->show();
	QObject::connect(model,SIGNAL(action(AbstractUnitAction*)),
			 drawer,SLOT(applyAction(AbstractUnitAction*)) );

	QObject::connect(drawer, SIGNAL(continueModel()),
			 model, SLOT(next()) );

	model->next();
    }
};

#endif // MAINHELPER_H
