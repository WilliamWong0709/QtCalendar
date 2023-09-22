#ifndef QTCLD_H
#define QTCLD_H

#include "xlabel.h"

//#include <QtWidgets/qmainwindow.h>
//#include <QtCore/qdatetime.h>
//#include <QtWidgets/qspinbox.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qgridlayout.h>
#include <QMainWindow>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QMap>
#include <QDate>
#include <QString>

static const int LABEL_COUNT_MAX = 49;
static const QString filePath("cld_data.dat");

class QtCld : public QMainWindow
{
	Q_OBJECT

public:
	QtCld(QWidget *parent = 0);
	~QtCld();

protected:
	void closeEvent(QCloseEvent *e) override;

private:
	QDate mDisplayDate;
	QDate mTodayDate;
	QWidget *mCentralWidget;
	XLabel *mLabels[49];
	int mHighlightLabelIndex;
	QSpinBox *mYearSpinBox;
	QComboBox *mMonthComboBox;
	QSpinBox *mFontSizeSpinBox;
	QGridLayout *mCldLayout;
	

	void initCld();
	void saveToFile(QString filepath);
	void readFromFile(QString filepath);

private slots:
	void updateCld();
};

#endif // QTCLD_H
