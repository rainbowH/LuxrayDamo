#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void TcpClientSocket::dataReceived()
{
    while (bytesAvailable()>0)
    {
        int length = bytesAvailable();
        char buf[1024];
        read(buf, length);

        QString msg = buf;
        emit updateClient(msg, length);
    }

}

void TcpClientSocket::onDisconnected()
{
    emit disConnected(this->socketDescriptor());
}
