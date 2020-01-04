#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

class TcpClientSocket :public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent = 0);

signals:
    void disconnected(qintptr);
    void updateClients(QString);

protected slots:
    void dataReceived();
    void slotDisconnect();

};

#endif // TCPCLIENTSOCKET_H
