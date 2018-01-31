#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "tcpclientsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = 0, int port = 8890);
//    QList<TcpClientSocket*> tcpClientSocketList;
    TcpClientSocket *tcpClientSocket;

signals:
    void updateServer(QString, int);

public slots:
    void onDisconnected(int descriptor);
    void onReceivedMsg(QString msg, int length);

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // TCPSERVER_H
