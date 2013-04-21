#include "formationcontrol.h"
#include "../portVideoQt/portVideoQt.h"
#include "../portVideoQt/FrameInverter.h"
#include "newterminaldialog.h"
#include "openportsdialog.h"
#include "../ross/simulator.h"
#include <QtCore/QMutex>
#include <QtCore/QTimer>

#include <QtGui/QImage>

bool gGo;
int* gXPos;
int* gYPos;
float* gHeading;
QList<Terminal> terminalList;


FormationControl::FormationControl(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
   ui.setupUi(this);
   gGo = false;
   engine = new portVideoQt("Formation Control Demo",true,true, ui.cameraWidget);
   inverter = new FrameInverter();
   
   engine->addFrameProcessor(inverter);
   engine->start();

   QSize cameraSize = engine->getSize();
   ui.lstFormations->setSelectionMode(QAbstractItemView::SingleSelection);
   ui.lstFormations->setCurrentRow(0);

   connect(ui.actionNew_Terminal, SIGNAL(triggered()), this, SLOT(actionNewTerminalTriggered()));
   connect(ui.actionOpen_ports, SIGNAL(triggered()), this, SLOT(actionOpenPortsTriggered()));
   connect(ui.btnApplyFormation, SIGNAL(clicked()), this, SLOT(on_btnApplyFormation()));
}

void FormationControl::on_btnGo_clicked()
{
	if (env.areRobotsReady()) {
		Function f = formations[ui.lstFormations->currentRow()];
		env.initRobots(Formation(f, 0.09, Vector(), 0, 0, 0));
		changeFormation(ui.lstFormations->currentRow());
		//gGo = true;
		QTimer::singleShot(1000, this, SLOT(go()));
	} else {
		QMessageBox msgbox;
		msgbox.setWindowTitle("Robots not ready"); //this is ignore on mac
		msgbox.setText("Please fist try to enumerate robots or create dummy entries for simulation. To do this you need to click step 1 button."); //set message box text
		msgbox.addButton(QMessageBox::Ok); //add ok button
		msgbox.exec(); //display message box
	}
}

void FormationControl::on_btnApplyFormation_clicked()
{
	if (env.areRobotsReady()) {
		changeFormation(ui.lstFormations->currentRow());
	} else {
		QMessageBox msgbox;
		msgbox.setWindowTitle("Robots not ready"); //this is ignore on mac
		msgbox.setText("Please fist try to enumerate robots or create dummy entries for simulation. To do this you need to click step 1 button."); //set message box text
		msgbox.addButton(QMessageBox::Ok); //add ok button
		msgbox.exec(); //display message box

	}
}

void FormationControl::go()
{
	ui.btnGo->setDisabled(true);
	gGo = true;
}

void FormationControl::on_btnFindRobots_clicked()
{
	gXPos = new int[N_CELLS];
	gYPos = new int[N_CELLS];
	gHeading = new float[N_CELLS];
	gXPos[0] = 100;
	gYPos[0] = 100;
	gHeading[0] = 90;
	gXPos[1] = 130;
	gYPos[1] = 130;
	gHeading[1] = 90;
	gXPos[2] = 160;
	gYPos[2] = 160;
	gHeading[2] = 90;
	gXPos[3] = 190;
	gYPos[3] = 190;
	gHeading[3] = 90;
}

void FormationControl::actionNewTerminalTriggered()
{
   if(QDialog::Accepted == newTerminalDialog.exec())
   {
      Terminal terminal = newTerminalDialog.getTerminal();
      if(openTerminal(&terminal))
      {
         terminalList.push_back(terminal);
         //showTerminalWindow(terminalList.last());
      }
   }
   else
   {
   }
}

void FormationControl::actionOpenPortsTriggered()
{
	OpenPortsDialog* pOPDialog = new OpenPortsDialog(this);
	if(QDialog::Accepted == pOPDialog->exec())
	{
		
	}
	else
	{
	}
}

int FormationControl::openTerminal(Terminal* terminal)
{
	int isOpen = 0;
    if(terminal->type == TERMINAL_COM)
    {
        QString terminalString("COM");
        QString device = terminalString + QString::number(terminal->port);
        terminal->pSerPort = new QextSerialPort(device.toLocal8Bit().data(),
                                                terminal->portSettings,
                                                QextSerialPort::EventDriven);
        if(terminal->pSerPort)
        {
            connect(terminal->pSerPort, SIGNAL(readyRead()), this, SLOT(readRS232Terminal()));
            terminal->isOpen = isOpen = terminal->pSerPort->open(QextSerialPort::ReadWrite);
			terminal->pSerPort->write("H\r");
        }
        if(!isOpen)
        {
            QMessageBox::critical(0, qApp->tr("Critical"),
                device + qApp->tr(" not opened"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(0, qApp->tr("Information"),
                device + qApp->tr(" opened"), QMessageBox::Ok);
        }
    }
    return isOpen;
}

void FormationControl::removeTerminal(int index)
{
	terminalList.at(index).pSerPort->close();
	terminalList.removeAt(index);
}

void FormationControl::readRS232Terminal()
{
    QextSerialPort* port = (QextSerialPort*) QObject::sender();
    if(port)
    {
        QByteArray ba = port->readAll();
        if(ba.size())
        {
            Terminal* pTerm = getTerminalBySerPort(port);
        }
    }
}

QString portSettingsToText(PortSettings* portSettings)
{
	QString text;
    switch(portSettings->BaudRate)
    {
        case BAUD115200:
            text += "115200";
            break;
        case BAUD57600:
            text += "57600";
            break;
        case BAUD38400:
            text += "38400";
            break;
        case BAUD19200:
            text += "19200";
            break;
        case BAUD9600:
            text += "9600";
            break;
        case BAUD4800:
            text += "4800";
            break;
        case BAUD2400:
            text += "2400";
            break;
        case BAUD1200:
            text += "1200";
            break;
        default:
            break;
    };
    text += ":";
    switch(portSettings->DataBits)
    {
        case DATA_8:
            text += "8";
            break;
        case DATA_7:
            text += "7";
            break;
        case DATA_6:
            text += "6";
            break;
        case DATA_5:
            text += "5";
            break;
        default:
            break;
    };
    text += ":";
    switch(portSettings->Parity)
    {
        case PAR_NONE:
            text += "N";
            break;
        case PAR_EVEN:
            text += "E";
            break;
        case PAR_ODD:
            text += "O";
            break;
        default:
            break;
    };

    text += ":";
    switch(portSettings->StopBits)
    {
        case STOP_1:
            text += "1";
            break;
        case STOP_2:
            text += "2";
            break;
        default:
            break;
    };
    return text;
}

Terminal* FormationControl::getTerminalBySerPort(QextSerialPort* port)
{
    int i;
    for(i=0; i<terminalList.size(); ++i)
    {
        if(port)
        {
            if(port == terminalList.at(i).pSerPort)
            {
                return (Terminal*)&terminalList.at(i);
            }
        }
    }
    return NULL;
}

FormationControl::~FormationControl()
{
	delete gXPos;
	delete gYPos;
	delete gHeading;
   engine->running_ = false;
   engine->removeFrameProcessor(inverter);
   engine->quit();
   delete inverter;
   delete engine;
}
