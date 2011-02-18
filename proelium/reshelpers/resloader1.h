#ifndef RESLOADER1_H
#define RESLOADER1_H

#include <QObject>
#include <QPixmap>

class ResLoader1 : public QObject
{
Q_OBJECT
public:
    static void load1(QVector<QPixmap>&, int&, int&, QString,int,int, QColor);
    static QVector<QPixmap>* load2(QString,int,int, QColor);

signals:

public slots:

};

#endif // RESLOADER1_H
