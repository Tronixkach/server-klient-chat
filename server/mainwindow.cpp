#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpServer(nullptr)
{
    ui->setupUi(this);
    ui->onlineCountLabel->setText(QString("Клієнтів онлайн: %1").arg(clientSockets.size()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startServerButton_clicked()
{
    if (tcpServer && tcpServer->isListening()) {
        ui->textEdit_log->append("Сервер уже працює.");
        return;
    }

    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);

    int port = 1234;

    if (tcpServer->listen(QHostAddress::Any, port)) {
        ui->textEdit_log->append(QString("Сервер успішно запущено на порту %1...").arg(port));
        ui->startServerButton->setEnabled(false);
        ui->stopServerButton->setEnabled(true);

    } else {
        QMessageBox::critical(this, "Помилка", "Не вдалося запустити сервер: " + tcpServer->errorString());
    }
}



void MainWindow::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onClientReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::onClientDisconnected);

    clientSockets.append(clientSocket);
    ui->textEdit_log->append(QString("Нове підключення: Дескриптор сокета %1").arg(clientSocket->socketDescriptor()));
}
void MainWindow::onClientReadyRead()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    QString message = QString::fromUtf8(data);


    ui->textEdit_log->append("Отримано: " + message);

    for (QTcpSocket *client : clientSockets) {
        if (client->state() == QAbstractSocket::ConnectedState) {
            client->write(data);
        }
    }
}

void MainWindow::onClientDisconnected()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    clientSockets.removeOne(senderSocket);
    ui->textEdit_log->append("Клієнт відключився.");
    senderSocket->deleteLater();
}

void MainWindow::on_stopServerButton_clicked()
{
    if (tcpServer && tcpServer->isListening()) {


        for (QTcpSocket *client : clientSockets) {
            if (client->state() == QAbstractSocket::ConnectedState) {
                client->write("Система: Сервер зупиняє роботу. Бувай!");
                client->disconnectFromHost();
            }
        }
        clientSockets.clear();


        tcpServer->close();


        ui->textEdit_log->append("Сервер зупинено.");
        ui->onlineCountLabel->setText("Клієнтів онлайн: 0");

        ui->startServerButton->setEnabled(true);
        ui->stopServerButton->setEnabled(false);
    }
}