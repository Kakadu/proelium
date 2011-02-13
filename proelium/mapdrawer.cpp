#include <QDebug>
#include <QGraphicsScene>
#include "mapdrawer.h"
#include "GameMap.h"
#include <QGraphicsPixmapItem>
#include "reshelpers/resloader1.h"
#include <QPixmap>
#include <QVector>


MapDrawer::MapDrawer(QGraphicsScene* sc , GameMap* m) {
    if (TerrainSprites.count() == 0) {
	ResLoader1::load1(TerrainSprites,"main_terrain.pcx",9,9,QColor(255,0,255));
    }
    _imageHeight = TerrainSprites.at(0).height();
    _imageWidth = TerrainSprites.at(0).width();
    _map = m;
    _scene = sc;    
}

void MapDrawer::repaint() {
    int hc = _map->height(), wc = _map->width(),
	h = _imageHeight, w = _imageWidth,
	left=5, top=5;

    int k=0;
    for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
	    if (sq != NULL)  {
		QPixmap map = TerrainSprites.at(sq->terrainSprite());
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem(NULL, _scene);
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
		QPixmap map = TerrainSprites.at(sq->terrainSprite());
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem(NULL, _scene);
		item->setPixmap(map);
		item->setOffset(left+w*i,top+h*j);
		k++;
	    }
    }    
}

