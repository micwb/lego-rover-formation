#include "openportsdialog.h"
#include "types.h"
#include "formationcontrol.h"
#include "../qextserialport/qextserialport.h"

OpenPortsDialog::OpenPortsDialog(QWidget *parent)
	: QDialog(parent), p(parent)
{
	ui.setupUi(this);
	if(parent)
	{
		connect(ui.cmdClosePort, SIGNAL(clicked()), this, SLOT(closePort()));
		updateList(parent);
	}
}

void OpenPortsDialog::updateList(QWidget* parent)
{
	FormationControl* formCtrl = (FormationControl*) parent;
	QList<Terminal>& terminalList = formCtrl->getTerminalList();
	if(terminalList.size())
	{
		for(int i= 0; i<terminalList.size(); ++i)
		{
			QextSerialPort* port = terminalList.at(i).pSerPort;
			portList.push_back(port);
			ui.lstOpenPorts->addItem("COM" + QString::number(terminalList.at(i).port));
		}
		ui.lstOpenPorts->setCurrentRow(0, QItemSelectionModel::Select);
	}
}

void OpenPortsDialog::closePort()
{
	if(ui.lstOpenPorts->selectedItems().size())
	{
		int index = ui.lstOpenPorts->row(ui.lstOpenPorts->selectedItems().at(0));
		FormationControl* formCtrl = (FormationControl*) ((QWidget*)this->parentWidget());
		formCtrl->removeTerminal(index);
		ui.lstOpenPorts->clear();
		updateList(formCtrl);
	}
}

OpenPortsDialog::~OpenPortsDialog()
{

}
