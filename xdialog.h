#pragma once
#include <QDialog>
#include <QDate>

class XLabel;

class XDialog : public QDialog
{
	Q_OBJECT

public:
	XDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	void init(XLabel *xlabel, QDate date);
protected:
	void closeEvent(QCloseEvent *e) override;
private:
	XLabel *mXLabel;
	QDate mDate;
};

