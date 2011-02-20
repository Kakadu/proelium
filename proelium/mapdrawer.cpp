#include <QDebug>
#include <QGraphicsScene>
#include "mapdrawer.h"
#include "GameMap.h"
#include <QGraphicsPixmapItem>
#include "reshelpers/resloader1.h"
#include <QPixmap>
#include <QVector>
#include <reshelpers/rescontainer.h>
#include <iostream>
#include "reshelpers/gametextureitem.h"
#include "reshelpers/rescontainer.h"

using namespace std;

const QColor& MapDrawer::grayColor = QColor(255,0,255);

MapDrawer::MapDrawer(QGraphicsScene* sc , GameMap* m) {
    if (Sprites.count() == 0) {
	int x;
	TerrainPack *p = new TerrainPack;
	ResLoader1::load1(p->content,_imageHeight,_imageWidth, "main_terrain.pcx",9,9,grayColor);
	Sprites.insert(tr("main_terrain"),p);
	_imageHeight = p->content.at(0).height();
	_imageWidth =  p->content.at(0).width();

	UnitPack* tanks = new UnitPack;
	ResLoader1::load1(tanks->attack,x,x,
			  "./tank_att/TankAttackA_W.pcx",20,1,QColor(192,192,192) );
	Sprites.insert(tr("tank"),tanks);

	UnitPack* pturs = new UnitPack;
	ResLoader1::load1(pturs->attack,x,x,
			"./tank_att/TankAttackA_E.pcx",20,1,QColor(192,192,192) );

	Sprites.insert(tr("ptur"),pturs);
    }
    _map = m;
    _scene = sc;    

}
void MapDrawer::repaint() {
    int hc = _map->height(), wc = _map->width();
    int leftOffset = 5, topOffset=5;

    for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
	    if (sq==NULL)
		continue;
	    foreach (const Unit* u, sq->units) {
		GameTextureItem* item;

		if (unitGraphics.contains(u->id))
		    item = unitGraphics.value(u->id);
		else {
		    item = new GameTextureItem(_scene);
		    unitGraphics.insert(u->id,item);
		}
		UnitPack* pack = dynamic_cast<UnitPack*>(Sprites.value(u->name));
		if (pack!=NULL) {
		    QPixmap norm = pack->attack.at(0);
		    int w = norm.width(), h = norm.height();
		    item->setOffset(leftOffset+i*_imageWidth + _imageWidth/2 - w/2,
				    topOffset+j*_imageHeight + _imageHeight/2 - h/2);
		    item->setPixmap(norm);
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
	    foreach (const Unit* u, sq->units) {
		GameTextureItem* item;

		if (unitGraphics.contains(u->id))
		    item = unitGraphics[u->id];
		else {
		    item = new GameTextureItem(_scene);
		    unitGraphics.insert(u->id,item);
		}
		UnitPack* pack = dynamic_cast<UnitPack*>(Sprites[u->name]);
		if (pack!=NULL) {
		    QPixmap norm = pack->attack.at(0);
		    int w = norm.width(), h = norm.height();
		    item->setOffset(leftOffset+i*_imageWidth + _imageWidth/2 - w/2,
				    topOffset+j*_imageHeight + _imageHeight/2 - h/2);
		    item->setPixmap(norm);
		}
	    }
	}
    qDebug() << "======== mapdrawer.cpp ================================";
    foreach (QString s, Sprites.keys()) {
	qDebug() << "=) " << s;
    }
}
void MapDrawer::placeArmies() {
    int w  = _map->width();
    int h = _map->height();
    int aaa[w+h+2][w+h+2];
    for (int j=0; j<=h; ++j) {
	Unit* unit = new Unit("tank",j);
	_map->getSquare1(w+1+j,j)->addUnit(unit);
	aaa[w+1+j][j]=1;

	unit = new Unit("ptur",1000+j);
	_map->getSquare1(j,w+1+j)->addUnit(unit);
    }
    /*
    for (int i=0;i<w+h+2;++i) {
	for (int j=0;j<w+h+2;++j) {
	    MapSquare* sq;
	    if ((sq = _map->getSquare1(i,j)) != NULL) {
		if (sq->units.count() >0)
		    cout << "9";
		else
		    cout << "1";
	    }
	    else cout << "0";
	}
	cout << "\n";
    } */

}

void MapDrawer::paintField() {
    int hc = _map->height(), wc = _map->width(),
	h = _imageHeight, w = _imageWidth,
	left=5, top=5;

    int k=0;
    SpritesPack* sp = Sprites["main_terrain"];
    qDebug() << "in paintField";
    foreach (QString s, Sprites.keys()) {
	qDebug() << "=) " << s;
    }

    TerrainPack* p = dynamic_cast<TerrainPack*>(sp);
    Images TerrainSprites = p->content;

    for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
	    if (sq != NULL)  {
		QPixmap map = TerrainSprites.at(40);
		GameTextureItem* item = new GameTextureItem( _scene);
		item->setPixmap(map);
		item->setOffset(left+w*i,top+j*h);
		k++;
	    }
    }

    //large lines
    left += w/2; top -= h/2;
    for (int i=0; i<hc+1; ++i)
	for (int j=0; j<wc+2; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+1+i-j,j+i);
	    if (sq!=NULL) {
		QPixmap map = TerrainSprites.at(18);
		GameTextureItem* item = new GameTextureItem(_scene);
		item->setPixmap(map);
		item->setOffset(left+w*i,top+h*j);
		k++;
	    }
    }
    qDebug() << "FUCKKKKKKKKKKKKKK!";
    if (Sprites.empty())
	qDebug() << " map is EMPTY";
    else {
	qDebug() << " map not empty";
	qDebug() << "in paintField";
	foreach (QString s, Sprites.keys()) {
	    qDebug() << "=) " << s;
	}
    }
    //_scene->removeItem();
}

