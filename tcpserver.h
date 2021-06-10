#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpServer; }
QT_END_NAMESPACE

class TcpServer : public QDialog
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

private:
    Ui::TcpServer *ui;
    QTcpServer *qTcpServer = nullptr;
    QVector<QString> randomMessages;

    void initServer();

private slots:
    void sendRandomMessage();
};
#endif // TCPSERVER_H
