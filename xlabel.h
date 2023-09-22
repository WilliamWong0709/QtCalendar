#pragma once
#include "dayscheduleui.h"
#include "xdialog.h"

#include <QLabel>
#include <QGridLayout>
#include <QTimeEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QGroupBox>
#include <QMouseEvent>
#include <QMap>

class XLabel : public QLabel
{
	Q_OBJECT

public:
	QDate mDate;

	XLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	XLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

	void dayScheduleSaveChange();

protected:
	void mouseReleaseEvent(QMouseEvent *e);

signals:
	void clicked();

private:
	//QMap<QString, QString> mDaySchedule;
	QVector<DayScheduleUi*> mDayScheduleUiVec;
	QVector<QLabel*> mLabelVec;
	QVector<QPushButton*> mModifyBtnVec;
	QVector<QPushButton*> mDeleteBtnVec;
	//QSignalMapper *mSignalMapper;
	QTimeEdit *mTimeEdit;
	QLineEdit *mLineEdit;
	QPushButton *mSaveBtn;
	QPushButton *mCancelBtn;
	QTime mTime;
	QString mActivityStr;
	XDialog *mEditDayScheduleDlg;
	QDialog *mChangeDlg;
	QGroupBox *mGrpBox;
	QString mBtnStyleSheet1;
	
	void editDaySchedule();
	void initChangeDlg(QTime, QString);
	void deleteActivitySilently(int index);

private slots:
	
	void changeActivity();
	void closeDlg();
	void insertActivity();
	void cancelInsertActivity();
	void modifyActivity();
	void deleteActivity();
	void updateStr();
	void updateDaySchedule();

};

