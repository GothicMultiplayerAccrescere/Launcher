#pragma once

#include "gmpclient.h"

class Server : public QObject
{
    Q_OBJECT
public:
    enum Properties
    {
        P_Name,
        P_Url,
        P_Port,
        P_Nick,
        P_ServerName,
        P_Description,
        P_GameMode,
        P_ServerVersion,
        P_PlayerCount,
        P_BotCount,
        P_Ping,
        P_Max
    };

    Server();

    virtual ~Server() override;

    inline void setName(const QString &name) { m_Name = name; }
    inline void setUrl(const QString &url) { m_Url = url; }
    inline void setPort(quint16 port) { m_Port = port; }
    inline void setNickname(const QString &nickname) { m_Nickname = nickname; }
    inline void setServerName(const QString &name) { m_ServerName = name; }
    inline void setDescription(const QString &description) { m_Description = description; }
    inline void setGameMode(const QString &gamemode) { m_GameMode = gamemode; }
    inline void setServerVersion(const QString &serverVersion) { m_ServerVersion = serverVersion; }
    inline void setPlayerCount(const QString &playerCount) { m_PlayerCount = playerCount; }
    inline void setBotCount(const QString &botCount) { m_BotCount = botCount; }
    inline void setPingCurrent(qint32 ping) { m_PingCurrent = ping; }

    inline const QString &name() const { return m_Name; }
    inline const QString &url() const { return m_Url; }
    inline quint16 port() const { return m_Port; }
    inline const QString &nickname() const { return m_Nickname; }
    inline const QString &serverName() const { return m_ServerName; }
    inline const QString &description() const { return m_Description; }
    inline const QString &gameMode() const { return m_GameMode; }
    inline const QString &serverVersion() const { return m_ServerVersion; }
    inline const QString &playerCount() const { return m_PlayerCount; }
    inline const QString &botCount() const { return m_BotCount; }
    inline QString ping() const { return QString::number(m_PingCurrent); }

public slots:
    void update();

private slots:
    void updateData(const ServerInfo &info);

signals:
    void updated();

private:
    QString m_Name;
    QString m_Url;
    quint16 m_Port;
    QString m_Nickname;
    QString m_ServerName;
    QString m_Description;
    QString m_GameMode;
    QString m_ServerVersion;
    QString m_PlayerCount;
    QString m_BotCount;
    qint64 m_PingCurrent;

    GMPClient *m_pClient;
};
