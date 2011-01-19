#include "framecollection.h"
#include <QBitmap>

FrameCollection::FrameCollection(QString prefix, int count, QColor color) {
    _backColor = color;
    _count = count;    
    images = new QPixmap*[count];
    for(int i=0; i<count; ++i) {
	images[i] = new QPixmap(prefix + int2str(i) + ".pcx");
	const QBitmap mask = images[i] -> createMaskFromColor(_backColor);
	images[i]->setMask(mask);
    }
}
QSize FrameCollection::resultSize = QSize(200,200);
QPixmap* FrameCollection::getImage(int x) {
    if (x<0 || x>= _count)
	return NULL;
    else return images[x];
}

QString FrameCollection::int2str(int x) {
    if (x<10)
	return "00" + QString::number(x,10);
    else if (x<100)
	return "0" + QString::number(x,10);
    else
	return QString::number(x,10);
}

FrameCollection::~FrameCollection() {
    for (int i=0; i< _count; ++i)
	delete images[i];
    delete images;
}
