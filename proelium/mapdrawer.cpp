#include <QDebug>
#include <QGraphicsScene>
#include "mapdrawer.h"
#include "GameMap.h"
#include <QGraphicsPixmapItem>
#include "reshelpers/terrainitem.h"

MapDrawer::MapDrawer(QGraphicsScene* sc , GameMap* m) {
    _map = m;
    _scene = sc;
    _sprites = new TerrainItem("main_terrain.pcx",9,9,QColor(255,0,255));
}
void MapDrawer::repaint() {
    int hc = _map->height(), wc = _map->width(),
	h = _sprites->imageHeight(), w = _sprites->imageWidth(),
	left=5, top=5;
    qDebug() << "w="<<w;
    qDebug() << "h="<<h;
    qDebug() << "wc="<<wc;
    qDebug() << "hc="<<hc;
    int k=0;
    for (int i=0; i<hc+2; ++i)
	for (int j=0; j<wc+1; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+i-j,j+i);
	    //if (sq != NULL)  {
		QPixmap map = _sprites->get(40);
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem(NULL, _scene);
		item->setPixmap(map);
		item->setOffset(left+w*i,top+j*h);
		k++;
	    //}
    }

    //large lines
    left += w/2; top -= h/2;
    for (int i=0; i<hc+1; ++i)
	for (int j=0; j<wc+2; ++j) {
	    MapSquare* sq = _map->getSquare1(wc+1+i-j,j+i);

		QPixmap map = _sprites->get(80);
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem(NULL, _scene);
		item->setPixmap(map);
		item->setOffset(left+w*i,top+h*j);
		k++;

    }
    qDebug() << "k= " << k;
}
