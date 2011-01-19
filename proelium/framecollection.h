#ifndef FRAMECOLLECTION_H
#define FRAMECOLLECTION_H
#include <QtGui/qwidget.h>
class FrameCollection {
private:
    QColor _backColor;
    QPixmap** images;
    int _count;
    static QString int2str(int);
public:
    static QSize resultSize;
    FrameCollection(QString, int, QColor color = QColor(192,192,192));
    ~FrameCollection();    
    QPixmap* getImage(int);
    int count() {
	return _count;
    }
};

#endif // FRAMECOLLECTION_H
