#pragma once

#include <QObject>
#include <QQueue>
#include <QHostAddress>

class QUdpSocket;

class GMPClient : public QObject
{
    Q_OBJECT
public:
    GMPClient(int serverId, QObject *pParent = nullptr);
    virtual ~GMPClient() override;

    void start(const QString &address, quint16 port);

signals:
    void serverChecked(int serverId, const QString &serverName, const QString &gamemode, const QString &version, const QString &player, const QString &bots, const QString &description);

private:
    int m_ServerId;
    QUdpSocket *m_pSocket;
};
