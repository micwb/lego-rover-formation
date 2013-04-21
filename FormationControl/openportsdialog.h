#ifndef OPENPORTSDIALOG_H
#define OPENPORTSDIALOG_H

#include <QDialog>
#include "GeneratedFiles/ui_openportsdialog.h"
#include "../qextserialport/qextserialport.h"

class OpenPortsDialog : public QDialog
{
	Q_OBJECT

public:
	OpenPortsDialog(QWidget *parent = 0);
	~OpenPortsDialog();

private slots:
	void closePort();
	void updateList(QWidget* parent);

private:
	QWidget* p;
	QList<QextSerialPort*> portList;

private:
	Ui::OpenPortsDialog ui;
};

#endif // OPENPORTSDIALOG_H
