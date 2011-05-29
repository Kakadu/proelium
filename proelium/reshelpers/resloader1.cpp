#include "resloader1.h"
#include <QPixmap>
#include <QBitmap>
#include <QDebug>

void ResLoader1::load1(QVector<QPixmap> &ans, int& height, int& width, QString path, int hcount, int vcount, QColor c) {
    QPixmap src(path);
    width = src.width() / hcount;
    height = src.height() / vcount;

    QBitmap mask = src.createMaskFromColor(c);
    src.setMask(mask);
    for (int i=0; i<hcount; ++i)
	for (int j=0; j<vcount; ++j) {
	    QPixmap item = src.copy(i*width, j*height, width, height);
	    ans.push_back(item);
	}
}

QVector<QPixmap>* ResLoader1::load2(QString path, int hcount, int vcount, QColor c) {
    QPixmap src(path);
    int width = src.width() / hcount,
	height = src.height() / vcount;

    QVector<QPixmap> *ans = new QVector<QPixmap>();
    QBitmap mask = src.createMaskFromColor(c);
    src.setMask(mask);
    for (int i=0; i<hcount; ++i)
	for (int j=0; j<vcount; ++j) {
	    QPixmap item = src.copy(i*width, j*height, width, height);
	    ans->push_back(item);
	}
    return ans;
}

void ResLoader1::load3(QVector<QPixmap> &ans, QString path, int hcount,
                       int vcount, QColor c) {
    QPixmap src(path);
    QBitmap mask = src.createMaskFromColor(c);
    int width = src.width() / hcount,
        height = src.height() / vcount;

    src.setMask(mask);
    for (int i=0; i<hcount; ++i)
        for (int j=0; j<vcount; ++j) {
            QPixmap item = src.copy(i*width, j*height, width, height);
            ans.push_back(item);
        }
}
