#pragma once

#include <QObject>
#include <QTimer>

#include <RakNet/RakPeerInterface.h>

enum class MessageIdentifiers : uint8_t
{
    GET_SERVER_INFO = 185
};

struct ServerInfo
{
    QString serverName;
    QString gamemode;
    QString version;
    QString player;
    QString bots;
    QString description;

    bool deserialize(const uint8_t *pData, size_t maxlen, size_t &seek);
};

class GMPClient : public QObject
{
    Q_OBJECT
public:
    GMPClient(QObject *pParent = nullptr);
    virtual ~GMPClient() override;

    void start(const QString &address, quint16 port);

public slots:
    void update();

signals:
    void serverChecked(const QString &serverName, const QString &gamemode, const QString &version, const QString &player, const QString &bots, const QString &description);

private:
    RakNet::RakPeerInterface *m_pClient;

    QTimer m_Timer;
};
