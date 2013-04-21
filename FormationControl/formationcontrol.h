#ifndef FORMATIONCONTROL_H
#define FORMATIONCONTROL_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_formationcontrol.h"
#include "../portVideoQt/portVideoQt.h"
#include "types.h"
#include "newterminaldialog.h"

class FormationControl : public QMainWindow
{
    Q_OBJECT

public:
    FormationControl(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~FormationControl();

	QList<Terminal>& getTerminalList() { return terminalList; };

public slots:
	void removeTerminal(int index);

private slots:
	void readRS232Terminal();

private slots:
	void go();
	void on_btnApplyFormation_clicked();
	void on_btnGo_clicked();
	void on_btnFindRobots_clicked();
	void actionNewTerminalTriggered();
	void actionOpenPortsTriggered();


private:
    NewTerminalDialog newTerminalDialog;

    int openTerminal(Terminal* terminal);
    QString portSettingsToText(PortSettings* portSettings);
    Terminal* getTerminalBySerPort(QextSerialPort* port);

private:
   FrameProcessor *inverter;
   portVideoQt *engine;
   QGraphicsScene* roboScene;

public:
    Ui::FormationControlClass ui;
};

#endif // FORMATIONCONTROL_H
