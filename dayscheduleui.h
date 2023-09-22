#pragma once
#include <QObject>
#include <QLabel>
#include <QPushButton>

class DayScheduleUi : public QObject
{
	Q_OBJECT

public:
	DayScheduleUi(QLabel *tl = nullptr, QLabel *al = nullptr, QPushButton *mb = nullptr, QPushButton *db = nullptr);
	~DayScheduleUi();

	QLabel *mTimeLbl;
	QLabel *mActivityLbl;
	QPushButton *mModifyBtn;
	QPushButton *mDeleteBtn;
};

