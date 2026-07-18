#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startServerButton_clicked();
    void on_stopServerButton_clicked();
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    QVector<QTcpSocket*> clientSockets;
};
#endif // MAINWINDOW_H
