#include "qtcld.h"
//#include <QtWidgets/QApplication>
#include <QApplication>

QMap<QDate, QMap<QTime, QString> > gSchedules;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	gSchedules = QMap<QDate, QMap<QTime, QString> >();
    QtCld w;
	w.resize(650, 530);
    w.show();
    return a.exec();
}
