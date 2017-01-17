#pragma once

#include <QObject>
#include <QHostAddress>

class QUdpSocket;

class GMPClient : public QObject
{
    Q_OBJECT
public:
    GMPClient(QObject *pParent = nullptr);
    virtual ~GMPClient() override;

    void start(const QString &address, quint16 port);

public slots:
    void readyRead();
    void connected();

signals:
    void serverChecked(const QString &serverName, const QString &gamemode, const QString &version, const QString &player, const QString &bots, const QString &description);

private:
    QUdpSocket *m_pSocket;
};
