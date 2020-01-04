#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "server.h"

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
    int port;
    Server *server;
public slots:
    void on_pbCreat_clicked();
    void updateServer(QString);
    void updateUsername(QString);
    void on_pbClose_clicked();
};

#endif // MAINWINDOW_H
