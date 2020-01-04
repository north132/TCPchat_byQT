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
    //connect(tcpClientSocket, SIGNAL(updateClients(QString,qint64)), this, SLOT(updateClients(QString,qint64)));
    connect(tcpClientSocket, SIGNAL(updateClients(QString)), this, SLOT(updateClients(QString)));
    connect(tcpClientSocket, SIGNAL(disconnected(qintptr)), this, SLOT(slotDisconnected(qintptr)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);

}

//收到消息时处理消息
void Server::updateClients(QString msg)
{
    QString Msg = msg.section('#',0,-2);
    QString Ctl = msg.section('#',-1);

    if(Ctl=="00")
    {
        //QString temp = "#99";
        emit updateUsername(msg);
    }
    else if(Ctl=="01")
    {
        emit updateUsername(msg);
    }
    else if(Ctl=="02")
    {
        emit updateServer(Msg);
    }
    else if(Ctl=="03")
    {
        for(int i=0; i<tcpClientSocketList.count(); i++)    //广播到各个客户端
        {
            QTcpSocket *item = tcpClientSocketList.at(i);
            item->write(msg.toLocal8Bit());
        }
        QString sender = Msg.section(' ',0,0);
        QString receiver = Msg.section('%',-1);
        Msg = Msg.section('%',0,0);
        Msg = Msg.section(':',-1);
        Msg = sender + " send to " + receiver + ":" + Msg;
        emit updateServer(Msg);
        return;
    }
    for(int i=0; i<tcpClientSocketList.count(); i++)    //广播到各个客户端
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        item->write(msg.toLocal8Bit());
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
