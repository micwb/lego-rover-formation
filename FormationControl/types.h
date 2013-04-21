#ifndef TYPES_H
#define TYPES_H

#include <QtCore/QElapsedTimer>
#include <qextserialport.h>
#include <QextSerialEnumerator.h>

#define SIMULATOR_TIME_INTERVAL_MS  100.0
#define SIMULATOR_TIME_INTERVAL_SEC (SIMULATOR_TIME_INTERVAL_MS / 1000.0)
#define STI_MS                      SIMULATOR_TIME_INTERVAL_MS
#define STI_SEC                     SIMULATOR_TIME_INTERVAL_SEC

#define TERMINAL_COM 1
#define TERMINAL_TCP 2
#define TERMINAL_UDP 3

class TerminalDockWidget;

typedef struct _Terminal
{
    int                 type;
    int                 port;
    bool                isOpen;
    QString             label;
    PortSettings        portSettings; //only for COM terminal
    QextSerialPort*     pSerPort;     //only for COM terminal
} Terminal;

extern QList<Terminal> terminalList;
extern int gCameraScalePPM;
extern QElapsedTimer* pElapsedTimer;

#endif // TYPES_H
