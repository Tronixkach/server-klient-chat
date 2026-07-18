#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);


    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(ui->lineEdit_message, &QLineEdit::returnPressed, this, &MainWindow::on_sendButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {

        QString ip = ui->lineEdit_ip->text();
        int port = ui->spinBox_port->value();

        if (ui->lineEdit_name->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Помилка", "Будь ласка, введіть свій нікнейм!");
            return;
        }

        socket->connectToHost(ip, port);
    } else {
        socket->disconnectFromHost();
    }
}


void MainWindow::on_sendButton_clicked()
{
    QString messageText = ui->lineEdit_message->text().trimmed();
    if (messageText.isEmpty()) return;

    QString fullMessage = ui->lineEdit_name->text() + ": " + messageText;

    socket->write(fullMessage.toUtf8());
    ui->lineEdit_message->clear();
}


void MainWindow::onConnected()
{
    ui->textEdit_chat->append("Система: Успішно підключено до сервера!");
    ui->connectButton->setText("Відключитися");

    ui->lineEdit_ip->setEnabled(false);
    ui->spinBox_port->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
}


void MainWindow::onDisconnected()
{
    ui->textEdit_chat->append("Система: З'єднання з сервером розірвано.");
    ui->connectButton->setText("Підключитися");


    ui->lineEdit_ip->setEnabled(true);
    ui->spinBox_port->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);
}


void MainWindow::onReadyRead()
{

    QByteArray data = socket->readAll();
    ui->textEdit_chat->append(QString::fromUtf8(data));
}