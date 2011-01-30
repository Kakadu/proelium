#ifndef RESLOADER1_H
#define RESLOADER1_H

#include <QObject>
#include <QPixmap>

class ResLoader1 : public QObject
{
Q_OBJECT
public:
    //explicit ResLoader1();
    static QVector<QPixmap>* load1(QString,int,int, QColor);
signals:

public slots:

};

#endif // RESLOADER1_H
