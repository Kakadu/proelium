#include <QDebug>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QVector>
#include "reshelpers/resloader1.h"
#include "reshelpers/rescontainer.h"
#include "textures/UnitTextureItem.h"
#include "textures/TerrainTextureItem.h"
#include "action/UserActionHypervisor.h"
#include "stuff/abstractions.h"
#include "mapdrawer.h"
#include "GameMap.h"

const QColor& MapDrawer::grayColor = QColor(255,0,255);
extern UserActionHyperVisor* MainHyperVisor;
extern QMap<QString, SpritesPack*> Sprites;
extern QVector<QString> dirHelper;

MapDrawer::MapDrawer(QGraphicsScene* sc, GameMap* m) {
    LEFT_OFFSET = TOP_OFFSET = 5;
    _map = m;
    _scene = sc;
    cursorT = NULL;

	if (Sprites.count() != 0)
		return;

	TerrainPack *p = new TerrainPack;
	ResLoader1::load1(p->content,_imageHeight,_imageWidth,
						  ":/main_terrain",9,9,grayColor);
	Sprites.insert(tr("main_terrain"),p);

	_imageHeight = p->content.at(0).height();
	_imageWidth =  p->content.at(0).width();

    Sprites.insert(tr("Samurai"),
                   ResLoader1::addSprites(":/Samurai/Run_", 10, ":/Samurai/Death_", 15, ":/Samurai/AttackA_", 15));
    Sprites.insert(tr("AfricanChampion"),
                   ResLoader1::addSprites(":/AfricanChampion/Run_", 10, ":/AfricanChampion/Death_", 15, ":/AfricanChampion/Attack_", 15));
    TerrainPack* cursor = new TerrainPack();
    ResLoader1::load3(cursor->content,":/cursor",30,1,QColor(192,192,192));
    Sprites.insert(tr("cursor"), cursor);
}
/**
  Получить по клетке в карте её экранные координаты.
  */
QPoint MapDrawer::screenCoords(int x, int y) {
    int wc = _map->width(),
        w = _imageWidth,
        h = _imageHeight;

	if ((x+y-wc) % 2 == 0) {
		// первый вариант
		int i = (x+y-wc)/2, j = y-i;
		return QPoint(LEFT_OFFSET+w*i,TOP_OFFSET+h*j);
	} else if ((x+y-wc) % 2 == 1) {
		int i = (x+y-wc-1)/2, j = y-i;
		return QPoint(LEFT_OFFSET+w*i+w/2,TOP_OFFSET+h*j-h/2);
	}
	throw "cant get screen coords";
}

/**
  Перерисовка юнитов
  */
void MapDrawer::repaint()  {
    int hc = _map->height(), wc = _map->width();

	for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
		//Q_ASSERT(check_squares(i,j,wc+i-j,j+i));
		MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
		if (sq==NULL)
			continue;
		QPoint terrLoc = screenCoords(wc+i-j,j+i);
		//TODO: итериться наверное не стоит, надо рисовать только верхний элемент
		foreach (Unit* u, sq->units) {
			UnitTextureItem* item;

			if (unitGraphics.contains(u->id))
				item = unitGraphics.value(u->id);
			else {
				item = new UnitTextureItem(_scene, u);
				unitGraphics.insert(u->id, item);
			}

            UnitPack* pack = dynamic_cast<UnitPack*>(Sprites.value(u->name));
            if (pack==NULL)
                continue;

            if (u->alive()) {
                Game::Direction d = u->lastDir();
                qDebug() << pack->attack[d].count();
                QPixmap norm = pack->attack[d].at(0); // TODO: add default state
                item->setPixmap(norm);
                int w = norm.width(), h = norm.height();
                item->setOffset(terrLoc.x() - w/2 + _imageWidth/2,
                                terrLoc.y() - h/2 + _imageHeight/2);
            } else {
                    // remove UnitTextureItem
            }
        }
    }

	for (int i=0; i<hc+1; ++i)
	for (int j=0; j<wc+2; ++j) {
		MapSquare* sq = _map->getSquare1(wc+1+i-j,j+i);
		if (sq==NULL)
			continue;
		QPoint terrLoc = screenCoords(wc+i-j+1,j+i);
		foreach (Unit* u, sq->units) {
			UnitTextureItem* item;

			if (unitGraphics.contains(u->id))
				item = unitGraphics[u->id];
			else {
				item = new UnitTextureItem(_scene, u);
				unitGraphics.insert(u->id,item);
			}

            UnitPack* pack = dynamic_cast<UnitPack*>(Sprites[u->name]);
            if (pack==NULL)
                continue;
            if (u->alive()) {
                Game::Direction d = u->lastDir();
                QVector<QPixmap> temp =  pack->attack[d];
                qDebug() << pack->attack[d].count();
                QPixmap norm = temp.at(0);
                qDebug() << norm.isNull();
                item->setPixmap(norm);
                int w = norm.width(), h = norm.height();
                item->setOffset(terrLoc.x() - w/2 + _imageWidth/2,
                            terrLoc.y() - h/2 + _imageHeight/2);
            } else {
                        // TODO: remove UnitTextureItem
            }
        }
    }
}
/*
void MapDrawer::placeArmies(ModelParam *param) {
    // Расстановка армии. TODO: вынести в другое место.
    int w  = _map->width();
    int h = _map->height();
    const int s = w+h+2;
    Unit* unit;

    for (int j=0; j<=w; ++j) {
        //танки слева
        if (j<param->tankPlatoonCount/2){
            unit = new Unit("tank", _map->nextId());
            _map->getSquare1(j,w-j)->addUnit(unit);
            unit = new Unit ("tank", _map->nextId());
            _map->getSquare1(j,w-j+1)->addUnit(unit);
        } else {
            param->tankPlatoonCount=0;
        }

    //гаубицы справа
        if (j<param->d30PlatoonCount){
            unit = new Unit("d30",_map->nextId());
            _map->getSquare1(h+1+j,s-1-j)->addUnit(unit);
        } else {
            param->d30PlatoonCount = 0;
        }

    }
    if (param->tankPlatoonCount)
    param->tankPlatoonCount -= 2*(w+1);
}
*/
/**
  Отрисовка рельефа.
  */
void MapDrawer::paintField() {
	int hc = _map->height(), wc = _map->width(),
	h = _imageHeight, w = _imageWidth,
	left=5, top=5, s = hc+wc+2;

    int k=0;
    SpritesPack* sp = Sprites["main_terrain"];

    TerrainPack* p = dynamic_cast<TerrainPack*>(sp);
    Images TerrainSprites = p->content;

	for (int i=0; i<s; ++i)
	for (int j=0; j<s; ++j) {
		MapSquare* sq = _map->getSquare1(i,j);
		if (sq!=NULL)
		sq->setTerrainSprite(40);
	}
	MapSquare* sq;

    // правый ряд
    for (int j=0; j<=wc; ++j) {
        int i=hc+1;
        sq = _map->getSquare1(wc+i-j,i+j);
        if (sq!=NULL)
            sq->setTerrainSprite(27);
    }
    // правый внутренний ряд
    for (int j=1; j<=wc; ++j) {
        int i=hc;
        sq = _map->getSquare1(wc+1+i-j,i+j);
        if (sq!=NULL)
            sq->setTerrainSprite(39);
    }

    // слева
    for (int j=0; j<=wc; ++j) {
        int i=0;
        sq = _map->getSquare1(wc+i-j,i+j);
        if (sq!=NULL)
            sq->setTerrainSprite(1);
    }
    // слева 2й ряд
    for (int j=1; j<=wc; ++j) {
        int i=0;
        sq = _map->getSquare1(wc+1+i-j,i+j);
        if (sq!=NULL)
            sq->setTerrainSprite(13);
    }

    // сверху
    for (int i=0; i<=hc; ++i) {
        int j=0;
        sq = _map->getSquare1(wc+1+i,j+i);
        if (sq!=NULL)
            sq->setTerrainSprite(3);
    }

    // сверху второй ряд
    for (int i=1; i<=hc; ++i) {
        int j=0;
        sq = _map->getSquare1(wc+i,j+i);
        if (sq!=NULL)
            sq->setTerrainSprite(31);
    }

    // нижний внешний ряд
    for (int i=0; i<=hc; ++i) {
        int j=wc+1;
        sq = _map->getSquare1(i,j+i);
        if (sq!=NULL)
            sq->setTerrainSprite(9);
    }

    // нижний внутренний
    for (int i=0; i<hc; ++i) {
        int j=wc+1;
        sq = _map->getSquare1(i+1,j+i);
        if (sq!=NULL)
            sq->setTerrainSprite(37);
    }

	for (int i=0; i<hc+2; ++i)
		for (int j=0; j<wc+1; ++j) {
			MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
			if (sq != NULL)  {
				QPixmap map = TerrainSprites.at(sq->terrainSprite());
				TerrainTextureItem* item = new TerrainTextureItem(_scene);
				item->setPixmap(map);
				QPoint loc = screenCoords(wc+i-j,j+i);
				item->setOffset(loc.x(),loc.y());
				k++;
		}
	}

    //large lines
    left += w/2; top -= h/2;
    for (int i=0; i<hc+1; ++i)
    for (int j=0; j<wc+2; ++j) {
        MapSquare* sq = _map->getSquare1(wc+1+i-j,j+i);
        if (sq!=NULL) {
        QPixmap map = TerrainSprites.at(sq->terrainSprite());
                TerrainTextureItem* item = new TerrainTextureItem(_scene);
        item->setPixmap(map);
        QPoint loc = screenCoords(wc+1+i-j,j+i);
        item->setOffset(loc.x(),loc.y());
        k++;
        }
    }
}

void MapDrawer::evalCoords(int x, int y, int &i, int &j) {
    int wc = _map->width();
    /*
        find solution for two systems of equations
          x = wc+i-j,   i=[0;hc+1], j=[0;wc]
          y = i+j
        and
          x = wc+i-j+1, i=[0;hc],  j=[0;wc]
          y = i+j
        and return x,y
    */
    //int a= wc+x-y, b = x+y;
    int i1 = x+y-wc, j1 = y-x+wc, i2 = i1-1, j2=j1+1;
    bool v1 = (i1 % 2 == 0) && (j1 % 2 == 0),
         v2 = (i2 % 2 == 0) && (j2 % 2 == 0);
    if (v1 && !v2) {
        i = i1; j = j1;
    } else if (!v1 && v2) {
        i = i2; j = j2;
    } else {
        throw "problem in MapDrawer::evalCoords";
    }
    i/=2;
    j/=2;
    Q_ASSERT(i>=0 && j>=0);
}
/* assert helper for 1st type */
bool MapDrawer::check_squares(int i, int j, int x, int y) {
	int wc = _map->width();
	int u = wc+i-j, v = j+i;
	if (x!=u || y!=v)
		return false;
	int a,b;
	evalCoords(u,v,a,b);
	return (i==a && j==b);
}
/**
 * Передаем координаты как массива. Преобразуем их в "сковозные" и рисуем.
 */
void MapDrawer::showCursorSprite(const int &x, const int &y) {
    int i,j;
    evalCoords(x,y,i,j);
    if (cursorT == NULL) {
        cursorT = new TerrainTextureItem(_scene);
        auto pack = dynamic_cast<TerrainPack*>(Sprites["cursor"]);
        cursorT->setPixmap(pack->content[0]);
    }
    int w = cursorT->pixmap().width(),
        h = cursorT->pixmap().height();
    QPoint loc = screenCoords(x,y);
    cursorT->setOffset(loc.x()-w/2+_imageWidth/2, loc.y()-h/2+_imageHeight/2);
}
