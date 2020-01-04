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
//        qint64 length = bytesAvailable();
//        char buf[1024];
//        read(buf,length);

//        QString msg = buf;
//        emit updateClients(msg,length);
        QByteArray array=this->readAll();//从通信套接字中取出内容
        //ui.textEditRead->append(QString::fromLocal8Bit(array));//在文本框中追加
        emit updateClients(QString::fromLocal8Bit(array));
    }
}

void TcpClientSocket::slotDisconnect()
{
    emit disconnected(this->socketDescriptor());
}
