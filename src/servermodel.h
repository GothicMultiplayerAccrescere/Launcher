#pragma once

#include <QAbstractTableModel>

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
        P_Player,
        P_PlayerCurrent,
        P_Bots,
        P_BotsCurrent,
        P_Ping,
        P_Max
    };

    inline void setName(const QString &name) { m_Name = name; }
    inline void setUrl(const QString &url) { m_Url = url; }
    inline void setPort(quint16 port) { m_Port = port; }
    inline void setServerName(const QString &name) { m_ServerName = name; }
    inline void setDescription(const QString &description) { m_Description = description; }
    inline void setPlayerCount(quint64 playerCount) { m_Player = playerCount; }
    inline void setPlayerCountCurrent(quint64 playerCountCurrent) { m_PlayerCurrent = playerCountCurrent; }
    inline void setBotCount(quint64 botCount) { m_Bots = botCount; }
    inline void setBotCountCurrent(quint64 botCountCurrent) { m_BotsCurrent = botCountCurrent; }
    inline void setPintCurrent(quint64 ping) { m_PingCurrent = ping; }

    inline const QString &name() const { return m_Name; }
    inline const QString &url() const { return m_Url; }
    inline quint16 port() const { return m_Port; }
    inline const QString &serverName() const { return m_ServerName; }
    inline const QString &description() const { return m_Description; }
    inline QString playerCount() const { return QString::number(m_PlayerCurrent) + "/" + QString::number(m_Player); }
    inline QString botCount() const { return QString::number(m_BotsCurrent) + "/" + QString::number(m_Bots); }
    inline QString ping() const { return QString::number(m_PingCurrent); }

private:
    QString m_Name;
    QString m_Url;
    quint16 m_Port;
    QString m_ServerName;
    QString m_Description;
    quint64 m_Player;
    quint64 m_PlayerCurrent;
    quint64 m_Bots;
    quint64 m_BotsCurrent;
    quint64 m_PingCurrent;
};

class ServerModel : public QAbstractTableModel
{
public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QVector<Server> m_Server;
};
