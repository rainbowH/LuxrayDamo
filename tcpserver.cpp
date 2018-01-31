#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent, int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    qDebug() <<"开始监听";
    connect(this, SIGNAL(newConnection()), this, SLOT());
}

void TcpServer::onDisconnected(int descriptor)
{
    delete tcpClientSocket;
}

void TcpServer::onReceivedMsg(QString msg, int length)
{
    emit updateServer(msg, length);
}

void TcpServer::incomingConnection(int socketDescriptor)
{
    tcpClientSocket = new TcpClientSocket(this);
    connect(tcpClientSocket, SIGNAL(disConnected(int)), this, SLOT(onDisconnected(int)));
    connect(tcpClientSocket, SIGNAL(updateClient(QString,int)), this, SLOT(onReceivedMsg(QString,int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
}
