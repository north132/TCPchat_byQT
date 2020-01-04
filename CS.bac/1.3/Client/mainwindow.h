#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>
#include <string.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool status;
    int port;
    QHostAddress *serverIP;
    QString username;
    QTcpSocket *tcpSocket;
    void getMsg(QString);

public slots:
    void on_pbEnter_clicked();
    void on_pbSend_clicked();
    void slotConnect();
    void slotDisconnect();
    void dataReceived();
    void on_pbPrivateSend_released();
};

#endif // MAINWINDOW_H
