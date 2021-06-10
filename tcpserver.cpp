#include "tcpserver.h"
#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    initServer();
    //randMessages
    for(int i=0; i<20; i++){
        randomMessages << "mensaje aleatorio" + QString::number(i);
    }
    // connect
    connect(ui->btn_close, &QAbstractButton::clicked, this, &QWidget::close);
    //connect for the socket server
    connect(qTcpServer, &QTcpServer::newConnection, this, &TcpServer::sendRandomMessage);

}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::initServer()
{
    qInfo() << "initServer";
    qTcpServer = new QTcpServer(this);
    if(!qTcpServer->listen()){
        qInfo() << "ServerError: " + qTcpServer->errorString();
        return;
    }
    QString port = QString::number(qTcpServer->serverPort(), 10);
    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i=0; ipAddressList.size(); i++){
        if(ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()){
            ipAddress = ipAddressList.at(i).toString();
            qInfo() << "ip: " << ipAddress;
            break;
        }
    }
    if(ipAddressList.isEmpty()){
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    QString info = "ip: " + ipAddress + ", puerto = " + port;
    ui->lbl_info->setText(info);
}

void TcpServer::sendRandomMessage()
{
    qInfo() << "SendRandomMessage";
    QByteArray qByteArray;
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly);
    QString randomMessage = randomMessages[QRandomGenerator::global()->bounded(randomMessages.size())];
    qDataStream << randomMessage;
    QTcpSocket *clientConnection = qTcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    clientConnection->write(qByteArray);
    clientConnection->disconnectFromHost();
    ui->lbl_message->setText(randomMessage);
}

