#include "server.h"

Server::Server(QObject *parent,int port)    //Server端初始化时开始监听端口
    :QTcpServer(parent)
{
    listen(QHostAddress::Any,port);
}

//出现新连接时
void Server::incomingConnection(qintptr socketDescriptor)
{
    TcpClientSocket * tcpClientSocket = new TcpClientSocket(this);
    connect(tcpClientSocket, SIGNAL(updateClients(QString,qint64)), this, SLOT(updateClients(QString,qint64)));
    connect(tcpClientSocket, SIGNAL(disconnected(qintptr)), this, SLOT(slotDisconnected(qintptr)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);
}

//将任意客户端发来的信息进行广播
void Server::updateClients(QString msg,qint64 length)
{
    emit updateServer(msg,length);      //向主窗口发信号更新消息。
    for(int i=0; i<tcpClientSocketList.count(); i++)    //广播到各个客户端
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(msg.toLatin1(),length) != length)
        {
            continue;
        }
    }
}

//将断开连接的TcpSocket对象删除
void Server::slotDisconnected(qintptr descriptor)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            tcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}
