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
        //int length=0;
        QString msg=username+tr(":Leave Chat Room");
//        if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg. length())
//        {
//            return;
//        }
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

    QString msg = username+":"+ui->lESend->text();

    //tcpSocket->write(msg.toLatin1(),msg.length());
    tcpSocket->write(msg.toLocal8Bit());
    ui->lESend->clear();
}

void MainWindow::slotConnect()  //connected信号。连接成功后状态改变并广播信息。
{
    ui->pbSend->setEnabled(true);
    ui->pbEnter->setText(tr("离开聊天室"));

    int length = 0;
    QString msg = username + tr(":Enter the chatroom.");
//    if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
//    {
//        return;
//    }
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
//        QByteArray datagram;
//        datagram.resize(tcpSocket->bytesAvailable());

//        tcpSocket->read(datagram.data(),datagram.size());

//        QString msg=datagram.data();
//        ui->lwContent->addItem(msg.left(datagram.size()));
        QByteArray array=tcpSocket->readAll();//从通信套接字中取出内容
        //ui.textEditRead->append(QString::fromLocal8Bit(array));//在文本框中追加
        ui->lwContent->addItem(QString::fromLocal8Bit(array));
    }
}
