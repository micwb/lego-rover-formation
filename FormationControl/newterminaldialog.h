#ifndef NEWTERMINALDIALOG_H
#define NEWTERMINALDIALOG_H

#include <QtWidgets/QDialog>
#include <QtGui/QShowEvent>

#include "types.h"

namespace Ui {
    class NewTerminalDialog;
}

class NewTerminalDialog : public QDialog {
    Q_OBJECT
public:
    NewTerminalDialog(QWidget *parent = 0);
    ~NewTerminalDialog();

    Terminal getTerminal();
    QString getTerminalLabel();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::NewTerminalDialog *m_ui;

private:
    PortSettings portSettings;

    void setPortSettings();
    void enableCOMRelayted();
    void disableCOMRelayted();

private slots:
    void on_cmbType_currentIndexChanged(QString );
};

#endif // NEWTERMINALDIALOG_H
