#include "newterminaldialog.h"
#include "GeneratedFiles/ui_newterminaldialog.h"

NewTerminalDialog::NewTerminalDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::NewTerminalDialog)
{
    m_ui->setupUi(this);

}

NewTerminalDialog::~NewTerminalDialog()
{
    delete m_ui;
}

void NewTerminalDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NewTerminalDialog::showEvent(QShowEvent *e)
{
    if(m_ui->cmbType->currentText() == "COM")
    {
        m_ui->cmbPort->clear();
        QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
        if(!ports.isEmpty())
        {
            for(int i = 0; i < ports.size(); ++i)
            {
                m_ui->cmbPort->addItem(ports.at(i).portName.mid(3,2).toLocal8Bit().data());
            }
        }
        if(m_ui->cmbPort->count())
        {
            enableCOMRelayted();
        }
    }
    else
    {
        disableCOMRelayted();
    }
}

Terminal NewTerminalDialog::getTerminal()
{
    Terminal terminal;
    setPortSettings();
    if(m_ui->cmbType->currentText() == "COM")
    {
        terminal.type = TERMINAL_COM;
    }
    else
    {
        if(m_ui->cmbType->currentText() == "TCP")
        {
            terminal.type = TERMINAL_TCP;
        }
        else
        {
            terminal.type = TERMINAL_UDP;
        }
    }
    terminal.port = m_ui->cmbPort->currentText().toInt();
    terminal.portSettings = portSettings;
    terminal.label = m_ui->lineLabel->text();
    return terminal;
}

QString NewTerminalDialog::getTerminalLabel()
{
    return m_ui->lineLabel->text();
}

void NewTerminalDialog::on_cmbType_currentIndexChanged(QString currentString)
{
    if(currentString == "COM")
    {
        enableCOMRelayted();
    }
    else
    {
        disableCOMRelayted();
    }
}

void NewTerminalDialog::enableCOMRelayted()
{
    m_ui->cmbBaud->setEnabled(true);
    m_ui->cmbData->setEnabled(true);
    m_ui->cmbParity->setEnabled(true);
    m_ui->cmbStop->setEnabled(true);
    m_ui->cmbControl->setEnabled(true);
}

void NewTerminalDialog::disableCOMRelayted()
{
    m_ui->cmbBaud->setEnabled(false);
    m_ui->cmbData->setEnabled(false);
    m_ui->cmbParity->setEnabled(false);
    m_ui->cmbStop->setEnabled(false);
    m_ui->cmbControl->setEnabled(false);
}

void NewTerminalDialog::setPortSettings()
{
    QString baudString;
    BaudRateType baud;
    baudString = m_ui->cmbBaud->currentText();
    if(baudString == "110")
        baud = BAUD110;
    else
    if(baudString == "300")
        baud = BAUD300;
    else
    if(baudString == "600")
        baud = BAUD600;
    else
    if(baudString == "1200")
        baud = BAUD1200;
    else
    if(baudString == "2400")
        baud = BAUD2400;
    else
    if(baudString == "4800")
        baud = BAUD4800;
    else
    if(baudString == "9600")
        baud = BAUD9600;
    else
    if(baudString == "19200")
        baud = BAUD19200;
    else
    if(baudString == "38400")
        baud = BAUD38400;
    else
    if(baudString == "57600")
        baud = BAUD57600;
    else
    if(baudString == "115200")
        baud = BAUD115200;
    else
        baud = BAUD115200;

    QString dataString;
    DataBitsType data;
    dataString = m_ui->cmbData->currentText();

    if(dataString == "5")
        data = DATA_5;
    else
    if(dataString == "6")
        data = DATA_6;
    else
    if(dataString == "7")
        data = DATA_7;
    else
    if(dataString == "8")
        data = DATA_8;
    else
        data = DATA_8;

    QString stopString;
    StopBitsType stop;
    stopString = m_ui->cmbStop->currentText();

    if(stopString == "1")
        stop = STOP_1;
    else
    if(stopString == "2")
        stop = STOP_2;
    else
        stop = STOP_2;

    QString parityString;
    ParityType parity;
    parityString = m_ui->cmbParity->currentText();

    if(parityString == "None")
        parity = PAR_NONE;
    else
    if(parityString == "Odd")
        parity = PAR_ODD;
    else
    if(parityString == "Even")
        parity = PAR_EVEN;
    else
        parity = PAR_NONE;

    QString controlString;
    FlowType control;
    controlString = m_ui->cmbControl->currentText();

    if(controlString == "Flow off")
        control = FLOW_OFF;
    else
    if(controlString == "Flow hardware")
        control = FLOW_HARDWARE;
    else
    if(controlString == "Flow xonoff")
        control = FLOW_XONXOFF;
    else
        control = FLOW_OFF;

    portSettings.BaudRate = baud;
    portSettings.DataBits = data;
    portSettings.FlowControl = control;
    portSettings.Parity = parity;
    portSettings.StopBits = stop;
    portSettings.Timeout_Millisec = 500;
}
