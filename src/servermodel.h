#pragma once

#include <QAbstractTableModel>
#include <QTimer>

#include "gmpclient.h"

class Server
{
public:
    enum Properties
    {
        P_Name,
        P_Url,
        P_Port,
        P_ServerName,
        P_Description,
        P_GameMode,
        P_ServerVersion,
        P_PlayerCount,
        P_BotCount,
        P_Ping,
        P_Max
    };

    inline void setName(const QString &name) { m_Name = name; }
    inline void setUrl(const QString &url) { m_Url = url; }
    inline void setPort(quint16 port) { m_Port = port; }
    inline void setServerName(const QString &name) { m_ServerName = name; }
    inline void setDescription(const QString &description) { m_Description = description; }
    inline void setGameMode(const QString &gamemode) { m_GameMode = gamemode; }
    inline void setServerVersion(const QString &serverVersion) { m_ServerVersion = serverVersion; }
    inline void setPlayerCount(const QString &playerCount) { m_PlayerCount = playerCount; }
    inline void setBotCount(const QString &botCount) { m_BotCount = botCount; }
    inline void setPintCurrent(quint64 ping) { m_PingCurrent = ping; }

    inline const QString &name() const { return m_Name; }
    inline const QString &url() const { return m_Url; }
    inline quint16 port() const { return m_Port; }
    inline const QString &serverName() const { return m_ServerName; }
    inline const QString &description() const { return m_Description; }
    inline const QString &gameMode() const { return m_GameMode; }
    inline const QString &serverVersion() const { return m_ServerVersion; }
    inline const QString &playerCount() const { return m_PlayerCount; }
    inline const QString &botCount() const { return m_BotCount; }
    inline QString ping() const { return QString::number(m_PingCurrent); }

private:
    QString m_Name;
    QString m_Url;
    quint16 m_Port;
    QString m_ServerName;
    QString m_Description;
    QString m_GameMode;
    QString m_ServerVersion;
    QString m_PlayerCount;
    QString m_BotCount;
    quint64 m_PingCurrent;
};

class ServerModel : public QAbstractTableModel
{
public:
    ServerModel(QObject *pParent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void startUpdates();

public slots:
    void updateRecords();

private slots:
    void updateServerRecord(int row, const QString &serverName, const QString &gamemode, const QString &version, const QString &playerCount, const QString &botCount, const QString &description);

private:
    QTimer m_Timer;
    QVector<QPair<Server, GMPClient *>> m_Server;
};
