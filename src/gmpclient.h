#pragma once

#include <QObject>
#include <QTimer>

#include <slikenet/peerinterface.h>

enum class MessageIdentifiers : uint8_t
{
    GET_SERVER_INFO = 189
};

struct ServerInfo
{
    QString serverName;
    QString gamemode;
    QString version;
    QString player;
    QString bots;
    QString description;
    int32_t averagePing;

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
    void serverChecked(const ServerInfo &info);

private:
    SLNet::RakPeerInterface *m_pClient;

    QTimer m_Timer;
};
