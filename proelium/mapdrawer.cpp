#include <QDebug>
#include <QPoint>
#include <QGraphicsScene>
#include "mapdrawer.h"
#include "GameMap.h"
#include <QGraphicsPixmapItem>
#include "reshelpers/resloader1.h"
#include <QPixmap>
#include <QVector>
#include <reshelpers/rescontainer.h>
#include "reshelpers/gametextureitem.h"
#include "reshelpers/rescontainer.h"
#include "GlobalConst.h"
#include "textures/UnitTextureItem.h"
#include "textures/TerrainTextureItem.h"
#include "action/UserActionHypervisor.h"

const QColor& MapDrawer::grayColor = QColor(255,0,255);
extern UserActionHyperVisor* MainHyperVisor;

MapDrawer::MapDrawer(QGraphicsScene* sc, GameMap* m) {
    LEFT_OFFSET = TOP_OFFSET = 5;
    aniGroup = new AniGroup(this);
    QObject::connect(aniGroup,SIGNAL(finished()),
		     this,SLOT(endVisiting()));

    if (Sprites.count() == 0) {
	int x;
	TerrainPack *p = new TerrainPack;
	ResLoader1::load1(p->content,_imageHeight,_imageWidth,
                          ":/main_terrain.png",9,9,grayColor);
	Sprites.insert(tr("main_terrain"),p);
	_imageHeight = p->content.at(0).height();
	_imageWidth =  p->content.at(0).width();

	UnitPack* tanks = new UnitPack;
	ResLoader1::load1(tanks->attack,x,x,
                          ":/tank_att/TankAttackA_E.png",20,1,QColor(192,192,192) );
	ResLoader1::load1(tanks->move,x,x,
                          ":/tank_run/TankRun_E.png",14,1,QColor(192,192,192) );
	ResLoader1::load1(tanks->death,x,x,
                          ":/tank_death/TankDeath_E.png",15,1,QColor(192,192,192) );
	tanks->normal = tanks->attack;
	Sprites.insert(tr("tank"),tanks);
	/*int c=0;
	foreach(const QPixmap p, tanks->move) {
	    if (p.isNull()) {
		qDebug() << "image corrupted! n = " << c;
	    }
	    c++;
	}*/

	//TODO: load new resources in future
	UnitPack* d30 = new UnitPack;
	ResLoader1::load1(d30->attack,x,x,
                        ":/artillery/ArtilleryAttackA_W.png",12,1,QColor(192,192,192) );
	ResLoader1::load1(d30->death,x,x,
                        ":/artillery/ArtilleryDeath_W.png",12,1, QColor(192,192,192));

	d30->normal = d30->attack;
	Sprites.insert(tr("d30"),d30);

	//TODO: load new resources in future
	UnitPack* pturs = new UnitPack;
	ResLoader1::load1(pturs->attack,x,x,
                        ":/ptur/TOW_Infantry_Attack_W.png",15,1,QColor(192,192,192) );
	ResLoader1::load1(pturs->death,x,x,
                        ":/ptur/TOW_Infantry_Death_W.png",15,1, QColor(192,192,192));
	pturs->normal = pturs->attack;
	Sprites.insert(tr("ptur"),pturs);


    }
    _map = m;
    _scene = sc;    

}
/**
  Получить по клетке в карте её экранные координаты.
  */
QPoint MapDrawer::screenCoords(int x, int y) {
    int wc = _map->width(),
	w = _imageWidth,
	h  = _imageHeight;

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
    int leftOffset = LEFT_OFFSET, topOffset=TOP_OFFSET;

    for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
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
                        QPixmap norm = pack->attack.at(0);
                        item->setPixmap(norm);
                        int w = norm.width(), h = norm.height();
                        item->setOffset(terrLoc.x() - w/2 + _imageWidth/2,
                                        terrLoc.y() - h/2 + _imageHeight/2);
                } else {
                    // remove UnitTextureItem
                }
	    }
	}

    leftOffset += _imageWidth/2;
    topOffset  -= _imageHeight/2;

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
                    //item = new GameTextureItem(_scene,_imageWidth,_imageHeight);
                    item = new UnitTextureItem(_scene, u);
		    unitGraphics.insert(u->id,item);
		}

		UnitPack* pack = dynamic_cast<UnitPack*>(Sprites[u->name]);
                if (pack==NULL)
                    continue;
                if (u->alive()) {
                        QPixmap norm = pack->attack.at(0);
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
