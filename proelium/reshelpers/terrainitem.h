#ifndef TERRAINITEM_H
#define TERRAINITEM_H
#include <QPixmap>
#include <QObject>

class TerrainItem : public QObject
{
Q_OBJECT
    QVector<QPixmap> *_images;
    int _hCount, _vCount;
    int _imageWidth, _imageHeight;
public:
    explicit TerrainItem(QString,int,int,QColor);
    int hCount() { return _hCount; }
    int vCount() { return _vCount; }
    int imageWidth() { return _imageWidth; }
    int imageHeight() { return _imageHeight; }

    QPixmap get(int i) {
	return _images->at(i);
    }
};

#endif // TERRAINITEM_H
