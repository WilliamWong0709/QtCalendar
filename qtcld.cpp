#include "qtcld.h"

#include <QApplication>
#include <QGroupBox>
#include <QDebug>
#include <QFile>
#include <QDataStream>

extern QMap<QDate, QMap<QTime, QString> > gSchedules;

QtCld::QtCld(QWidget *parent)
	: QMainWindow(parent),
	mCentralWidget(new QWidget),
	mHighlightLabelIndex(-1)
{
	mDisplayDate = mTodayDate = QDate::currentDate();

	QHBoxLayout *hLayout = new QHBoxLayout;
	QLabel *dateLabel = new QLabel(tr("Date:"));

	mMonthComboBox = new QComboBox;
	for (int i = 1; i <= 12; i++) {
		mMonthComboBox->addItem(QDate::longMonthName(i));
	}
	mMonthComboBox->setCurrentIndex(mTodayDate.month() - 1);
	connect(mMonthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCld()));

	mYearSpinBox = new QSpinBox;
	mYearSpinBox->setMinimum(1970);
	mYearSpinBox->setMaximum(2070);
	mYearSpinBox->setValue(mTodayDate.year());
	connect(mYearSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCld()));

	QLabel *fontSizeLabel = new QLabel(tr("Font size:"));

	mFontSizeSpinBox = new QSpinBox;
	mFontSizeSpinBox->setMinimum(16);
	mFontSizeSpinBox->setMaximum(24);
	mFontSizeSpinBox->setValue(16);
	connect(mFontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCld()));

	hLayout->addWidget(dateLabel);
	hLayout->addWidget(mMonthComboBox);
	hLayout->addWidget(mYearSpinBox);
	hLayout->addSpacing(24);
	hLayout->addWidget(fontSizeLabel);
	hLayout->addWidget(mFontSizeSpinBox);
	hLayout->addStretch(1);

	// Group Box
	QGroupBox *cldGroupBox = new QGroupBox;
	mCldLayout = new QGridLayout;

	initCld();
	updateCld();

	cldGroupBox->setLayout(mCldLayout);

	QVBoxLayout *centralLayout = new QVBoxLayout;
	centralLayout->addLayout(hLayout);
	centralLayout->addWidget(cldGroupBox, 1);
	mCentralWidget->setLayout(centralLayout);

	setCentralWidget(mCentralWidget);
}

QtCld::~QtCld()
{

}

void QtCld::closeEvent(QCloseEvent * e)
{
	saveToFile(filePath);
	e->accept();
}

void QtCld::saveToFile(QString filePath)
{
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly)) {
		QDataStream out(&file);
		out << gSchedules.size();
		for (auto it = gSchedules.begin(); it != gSchedules.end();it++) {
			out << it.key();
			out << it.value().size();
			for (auto itInner = it.value().begin(); itInner != it.value().end();itInner++) {
				out << itInner.key();
				out << itInner.value();
			}
		}
		file.close();
	}
	else {
		qDebug() << "Failed to open file for writing.";
	}
}

void QtCld::readFromFile(QString filePath)
{
	gSchedules.clear();

	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly)) {
		QDataStream in(&file);
		int schedulesSize, dayScheduleSize;
		QDate inDate;
		QTime inTime;
		QString inActivity;
		in >> schedulesSize;
		for (int i = 0; i < schedulesSize; i++) {
			in >> inDate;
			in >> dayScheduleSize;
			QMap<QTime, QString> daySchedule;
			for (int j = 0; j < dayScheduleSize; j++) {
				in >> inTime;
				in >> inActivity;
				daySchedule.insert(inTime, inActivity);
			}
			gSchedules.insert(inDate, daySchedule);
		}
		file.close();
	}
	else {
		qDebug() << "Failed to open file for reading.";
	}
}

void QtCld::initCld()
{
	readFromFile(filePath);

	mLabels[0] = new XLabel(tr("MON"));
	mLabels[1] = new XLabel(tr("TUE"));
	mLabels[2] = new XLabel(tr("WED"));
	mLabels[3] = new XLabel(tr("THU"));
	mLabels[4] = new XLabel(tr("FRI"));
	mLabels[5] = new XLabel(tr("SAT"));
	mLabels[6] = new XLabel(tr("SUN"));
	for(int i = 0;i < LABEL_COUNT_MAX;i++){
		if(i > 6)
			mLabels[i] = new XLabel(tr(""));
		mLabels[i]->setFixedSize(80, 80);
		mLabels[i]->setAlignment(Qt::AlignCenter);
	}
}

void QtCld::updateCld()
{
	QString styleSheetXlabel = QString("font:bold %1px;")
								.arg(mFontSizeSpinBox->value());

	mTodayDate = QDate::currentDate();
	mDisplayDate = QDate(mYearSpinBox->value(), mMonthComboBox->currentIndex() + 1, mTodayDate.day());
	bool isHighlight = mDisplayDate == mTodayDate;

	qDebug() << QString("%1 %2 %3").arg(mDisplayDate.year()).arg(mDisplayDate.month()).arg(mDisplayDate.day());

	QDate month1st(mDisplayDate.year(), mDisplayDate.month(), 1);
	int cursorFirstDay = month1st.dayOfWeek() - 1 + 7;
	int cursorLastDay = cursorFirstDay + mDisplayDate.daysInMonth();
	int cursorLastLabel = (cursorLastDay % 7 != 0 ? (cursorLastDay / 7 + 1) * 7 : cursorLastDay);
	int i, j, k;
	// Table head, MON, TUE, WED, etc
	for (i = 0; i < 7; i++) {
		mLabels[i]->mDate = QDate();
		mLabels[i]->setStyleSheet(styleSheetXlabel);
	}
	// Empty place holder
	for (i = 7; i < cursorFirstDay; i++) {
		mLabels[i]->mDate = QDate();
		mLabels[i]->setText(QString(""));
		mLabels[i]->setStyleSheet(styleSheetXlabel);
	}
	// Day 1 to Day 28/29/30/31
	for (i = cursorFirstDay, j = 1; i < cursorLastDay; i++, j++) {
		mLabels[i]->mDate = QDate(mDisplayDate.year(), mDisplayDate.month(), j);
		mLabels[i]->setText(QString::number(j));
		QString suffixStyleSheet("border:1px solid gray;");
		if (gSchedules.find(mLabels[i]->mDate) != gSchedules.end()) {
			suffixStyleSheet.append("text-decoration:underline;");
		}
		mLabels[i]->setStyleSheet(styleSheetXlabel + suffixStyleSheet);
	}
	// Empty place holder
	for (i = cursorLastDay; i < cursorLastLabel; i++) {
		mLabels[i]->mDate = QDate();
		mLabels[i]->setText(QString(""));
		mLabels[i]->setStyleSheet(styleSheetXlabel);
	}
	for (i = cursorLastLabel; i < LABEL_COUNT_MAX; i++) {
		mLabels[i]->mDate = QDate();
		mLabels[i]->setText(QString(""));
		mLabels[i]->setStyleSheet(styleSheetXlabel);
	}
	for (k = 0; k < cursorLastLabel; k++) {
		mCldLayout->addWidget(mLabels[k], k / 7, k % 7, 1, 1, Qt::AlignCenter);
	}
	
	/* Highlight today's label if this month is displayed, otherwise clear previous highlight label. */
	if (isHighlight) {
		mHighlightLabelIndex = month1st.dayOfWeek() - 1 + mTodayDate.day() - 1 + 7;
		QString newStyleSheet = mLabels[mHighlightLabelIndex]->styleSheet() + QString("background-color:yellow;");
		mLabels[mHighlightLabelIndex]->setStyleSheet(newStyleSheet);
	}
	else if(mHighlightLabelIndex >= 0 && mHighlightLabelIndex < LABEL_COUNT_MAX){
		QString oldStyleSheet = mLabels[mHighlightLabelIndex]->styleSheet();
		int index = oldStyleSheet.indexOf(QString("background-color"));
		QString newStyleSheet = oldStyleSheet.left(index);
		mLabels[mHighlightLabelIndex]->setStyleSheet(newStyleSheet);
		mHighlightLabelIndex = -1;
	}

}
