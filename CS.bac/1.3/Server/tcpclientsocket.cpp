#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnect()));
}

void TcpClientSocket::dataReceived()
{
    while(bytesAvailable()>0)
    {
        QByteArray array=this->readAll();//从通信套接字中取出内容
        emit updateClients(QString::fromLocal8Bit(array));
    }
}

void TcpClientSocket::slotDisconnect()
{
    emit disconnected(this->socketDescriptor());
}
