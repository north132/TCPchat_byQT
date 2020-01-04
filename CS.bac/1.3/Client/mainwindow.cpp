#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = false;
    port = 8010;
    ui->lEport->setText(QString::number(port));

    serverIP = new QHostAddress();

    ui->pbSend->setEnabled(false);
    ui->lEServerIP->setText(tr("127.0.0.1"));
}

MainWindow::~MainWindow()
{
    if(serverIP) delete serverIP;
    if(tcpSocket) delete tcpSocket;
    delete ui;
}

void MainWindow::on_pbEnter_clicked()
{
    //cout<< "Enter..." <<endl;
    if(!status)     //加入聊天室
    {
        QString ip = ui->lEServerIP->text();
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }

        if(ui->lEUsername->text()=="")
        {
            QMessageBox::information(this,tr("error"),tr("User name error!"));
            return;
        }

        username = ui->lEUsername->text();

        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnect()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnect()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));

        tcpSocket->connectToHost(*serverIP,port);

        status = true;

    }
    else    //离开聊天室
    {
        QString msg=username+tr(":Leave Chat Room")+tr("#01");
        tcpSocket->write(msg.toLocal8Bit());

        tcpSocket->disconnectFromHost();

        status=false;
    }
}

void MainWindow::on_pbSend_clicked()    //send键发送信息
{
    //cout<< "Send..." << endl;
    if(ui->lESend->text()=="")
        return;

    QString msg = username+":"+ui->lESend->text()+"#02";

    //tcpSocket->write(msg.toLatin1(),msg.length());
    tcpSocket->write(msg.toLocal8Bit());
    ui->lESend->clear();
}

void MainWindow::on_pbPrivateSend_released()
{
    if(ui->lEPrivate->text()=="")
        return;
    if(ui->lEPrivateID->text()=="")
    {
        QMessageBox::information(this,tr("error"),tr("ID is empty!"));
        return;
    }
    QString msg = username+" say to you:"+ui->lEPrivate->text()+"%"+ui->lEPrivateID->text()+"#03";
    tcpSocket->write(msg.toLocal8Bit());
    ui->lEPrivate->clear();
}

void MainWindow::slotConnect()  //connected信号。连接成功后状态改变并广播信息。
{
    ui->pbSend->setEnabled(true);
    ui->pbEnter->setText(tr("离开聊天室"));

    QString msg = username + tr(":Enter the chatroom.") + tr("#00");
    tcpSocket->write(msg.toLocal8Bit());
}

void MainWindow::slotDisconnect()   //disconnected信号,断开连接后的状态改变。
{
    ui->pbEnter->setText(tr("进入聊天室"));
    ui->pbSend->setEnabled(false);
}

void MainWindow::dataReceived()     //readyRead信号，接收消息.
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray array=tcpSocket->readAll();//从通信套接字中取出内容
        QString msg = QString::fromLocal8Bit(array);
        //ui->lwContent->addItem(msg);
//        QString front = msg.section('#',0,0);
//        QString behind = msg.mid(msg.indexOf('#'));
//        msg = front + behind.left(3);
        getMsg(msg);
//        behind = behind.remove(0,3);
//        while(!behind.isEmpty())
//        {
//            front = behind.section('#',0,0);
//            behind = msg.mid(msg.indexOf('#'));
//            msg = front + behind.left(3);
//            getMsg(msg);
//            behind = behind.remove(0,3);
//        }
    }
}

void MainWindow::getMsg(QString msg)
{
    QString Msg = msg.section('#',0,-2);
    QString Ctl = msg.section('#',-1);

    if(Ctl=="03")
    {
        QString sender = Msg.section(' ',0,0);
        QString receiver = Msg.section('%',-1);

        if(receiver==ui->lEUsername->text())
        {
            QString temp = Msg.section('%',0,-2);
            ui->lwContent->addItem(temp);
            return;
        }
        if(sender==ui->lEUsername->text())
        {
            Msg = Msg.section('%',0,0);
            Msg = Msg.section(':',-1);
            Msg = "You send to " + receiver + ":" + Msg;
            ui->lwContent->addItem(Msg);
            return;
        }
    }
    if(Ctl=="00")
    {
        ui->lwContent->addItem(Msg);
        ui->lwUsername->addItem(Msg.section(':',0,0));
        return;
    }
    if(Ctl=="01")
    {
        ui->lwContent->addItem(Msg);
        QString x = Msg.section(':',0,0);
        for(int i=0;i<ui->lwUsername->count();i++)
        {
            QString temp = ui->lwUsername->item(i)->text();
            if(x==temp)
            {
                QListWidgetItem * item = ui->lwUsername->takeItem(i);
                ui->lwUsername->removeItemWidget(item);
                break;
            }
        }
        return;
    }
    else if(Ctl=="02")
    {
        ui->lwContent->addItem(Msg);
        return;
    }

}
