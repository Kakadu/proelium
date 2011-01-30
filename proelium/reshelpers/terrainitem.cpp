#include "terrainitem.h"
#include "reshelpers/resloader1.h"

TerrainItem::TerrainItem(QString path, int w, int h, QColor c) {
    _hCount=w; _vCount = h;

    _images = ResLoader1::load1(path,w,h,c);
    _imageWidth = _images->at(0).width();
    _imageHeight = _images->at(0).height();
}
