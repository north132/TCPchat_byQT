#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = 8010;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(server) delete server;
}

void MainWindow::on_pbCreat_clicked()   //创建聊天室
{
    server = new Server(this,port);
    connect(server,SIGNAL(updateServer(QString,qint64)),this,SLOT(updateServer(QString,qint64)));

    ui->pbCreat->setEnabled(false);
}


void MainWindow::updateServer(QString msg,qint64 length)   //收到server的updateServer信号时，更新信息。刷新屏幕。
{
//    QString str = ui->lport->text();
//    QListWidgetItem *item = new QListWidgetItem;
//    item->setText(str);
//    ui->lwContent->addItem(item);
    ui->lwContent->addItem(msg.left(length));
}
