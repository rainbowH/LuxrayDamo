#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent = 0);

signals:
    void updateClient(QString, int);
    void disConnected(int);

protected slots:
    void dataReceived();
    void onDisconnected();
};

#endif // TCPCLIENTSOCKET_H
