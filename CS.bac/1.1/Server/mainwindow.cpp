#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = 8010;
    ui->pbClose->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(server) delete server;
}

void MainWindow::on_pbCreat_clicked()   //创建聊天室
{
    server = new Server(this,port);
    connect(server,SIGNAL(updateServer(QString)),this,SLOT(updateServer(QString)));

    ui->pbCreat->setEnabled(false);
    ui->pbClose->setEnabled(true);
    QString msg = "聊天室创建成功";
    ui->lwContent->addItem(msg);
}

void MainWindow::on_pbClose_clicked()   //关闭聊天室
{
    delete server;
    QString msg = "聊天室关闭";
    ui->lwContent->addItem(msg);

    ui->pbClose->setEnabled(false);
    ui->pbCreat->setEnabled(true);
}


void MainWindow::updateServer(QString msg)   //收到server的updateServer信号时，更新信息。刷新屏幕。
{
//    QString str = ui->lport->text();
//    QListWidgetItem *item = new QListWidgetItem;
//    item->setText(str);
//    ui->lwContent->addItem(item);
    ui->lwContent->addItem(msg);
}
