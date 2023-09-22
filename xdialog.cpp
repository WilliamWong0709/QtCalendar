#include "xdialog.h"
#include "xlabel.h"
#include <QDebug>
#include <QRegExp>

extern QMap<QDate, QMap<QTime, QString> > gSchedules;

XDialog::XDialog(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f),
	  mXLabel(nullptr),
	  mDate(QDate())
{

}

void XDialog::init(XLabel *xlabel, QDate date)
{
	mXLabel = xlabel;
	mDate = date;
}

void XDialog::closeEvent(QCloseEvent * e)
{
	qDebug() << "Calling XDialog::closeEvent";

	QString css = mXLabel->styleSheet();
	QRegExp rx("text-decoration\\s*:\\s*\\w+\\s*;");

	/* change mXLabel's appearnce if necessary. */
	if (gSchedules.find(mDate) != gSchedules.end()) {
		if (css.lastIndexOf(rx) >= 0)
			css.replace(rx, QString("text-decoration:underline;"));
		else
			css.append(QString("text-decoration:underline;"));
	}
	else {
		css.replace(rx, QString("text-decoration:none;"));
	}
	mXLabel->setStyleSheet(css);
}
