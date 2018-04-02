#include <QSettings>
#include <QDebug>

#include <functional>

#include "servermodel.h"
#include "server.h"

ServerModel::ServerModel(QObject *pParent) :
    QAbstractTableModel(pParent)
{
    QSettings s;
    m_Timer.setInterval(s.value("misc/refresh_rate", 10000).toInt());

    int rows = s.beginReadArray("server");
    for(int i = 0; i < rows; ++i)
    {
        s.setArrayIndex(i);
        Server *pServer = addServer(i);
        pServer->setName(s.value("server_name").toString());
        pServer->setUrl(s.value("server_url").toString());
        pServer->setNickname(s.value("server_nick").toString());
        pServer->setPort(static_cast<uint16_t>(s.value("server_port").toInt()));
    }
    s.endArray();
    emit dataChanged(index(0, 0), index(rowCount() - 1, Server::P_Max - 1));

    m_Timer.start();
    updateRecords();
}

int ServerModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_Server.size();
}

int ServerModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return Server::P_Max;
}

QVariant ServerModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    if(m_Server.size() <= index.row())
        return QVariant();

    const Server *server = m_Server[index.row()];
    switch(index.column())
    {
    case Server::P_Name:
        return server->name();
    case Server::P_Url:
        return server->url();
    case Server::P_Port:
        return server->port();
    case Server::P_Nick:
        return server->nickname();
    case Server::P_ServerName:
        return server->serverName();
    case Server::P_Description:
        return server->description();
    case Server::P_GameMode:
        return server->gameMode();
    case Server::P_ServerVersion:
        return server->serverVersion();
    case Server::P_PlayerCount:
        return server->playerCount();
    case Server::P_BotCount:
        return server->botCount();
    case Server::P_Ping:
        return server->ping();
    }

    return QVariant();
}

void ServerModel::updateData()
{
    QSettings s;
    s.beginWriteArray("Server");
    s.remove("");

    for(int i = 0, end = m_Server.size(); i < end; ++i)
    {
        s.setArrayIndex(i);
        s.setValue("server_name", m_Server[i]->name());
        s.setValue("server_url", m_Server[i]->url());
        s.setValue("server_port", m_Server[i]->port());
        s.setValue("server_nick", m_Server[i]->nickname());
    }

    s.endArray();
}

bool ServerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || index.parent().isValid() || index.row() >= m_Server.size() || index.column() >= Server::P_Max || (role != Qt::DisplayRole && role != Qt::EditRole))
        return false;

    Server *server = m_Server[index.row()];
    switch(index.column())
    {
    case Server::P_Name:
        server->setName(value.toString());
        updateData();
        return true;
    case Server::P_Url:
        server->setUrl(value.toString());
        updateData();
        return true;
    case Server::P_Port:
        server->setPort(static_cast<quint16>(value.toInt()));
        updateData();
        return true;
    case Server::P_Nick:
        server->setNickname(value.toString());
        updateData();
        return true;
    case Server::P_ServerName:
        server->setServerName(value.toString());
        return true;
    case Server::P_Description:
        server->setDescription(value.toString());
        return true;
    case Server::P_GameMode:
        server->setGameMode(value.toString());
        return true;
    case Server::P_ServerVersion:
        server->setServerVersion(value.toString());
        return true;
    case Server::P_PlayerCount:
        server->setPlayerCount(value.toString());
        return true;
    case Server::P_BotCount:
        server->setBotCount(value.toString());
        return true;
    case Server::P_Ping:
        server->setPingCurrent(value.toInt());
        return true;
    }

    return false;
}

bool ServerModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid() || row + count > m_Server.size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row, end = row + count; i < end; ++i)
        delete m_Server[i];

    m_Server.remove(row, count);

    updateData();
    endRemoveRows();
    return true;
}

QVariant ServerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case Server::P_Name:
            return tr("Alias");
        case Server::P_ServerName:
            return tr("Server name");
        case Server::P_GameMode:
            return tr("Gamemode");
        case Server::P_ServerVersion:
            return tr("Server version");
        case Server::P_PlayerCount:
            return tr("Player");
        case Server::P_BotCount:
            return tr("Bots");
        case Server::P_Ping:
            return tr("Ping");
        }
    }

    return QVariant();
}

void ServerModel::updateRecords()
{
    for(Server *pServer : m_Server)
        pServer->update();
}

void ServerModel::appendRecord(const QString &name, const QString &url, quint16 port)
{
    int index = rowCount();

    QSettings s;
    s.beginGroup("Misc");
    QString defaultNick = s.value("default_nick", "Nickname").toString();
    s.endGroup();
    s.beginWriteArray("Server");
    s.setArrayIndex(index);
    s.setValue("server_name", name);
    s.setValue("server_url", url);
    s.setValue("server_port", port);
    s.setValue("server_nick", defaultNick);
    s.endArray();

    beginInsertRows(QModelIndex(), index, index);
    Server *pServer = addServer(index);
    pServer->setName(name);
    pServer->setUrl(url);
    pServer->setPort(port);
    pServer->setNickname(defaultNick);
    endInsertRows();
}

Server *ServerModel::addServer(int row)
{
    Server *pServer = new Server;
    m_Server.insert(row, pServer);
    connect(&m_Timer, &QTimer::timeout, pServer, &Server::update);
    connect(pServer, &Server::updated, std::bind(&ServerModel::dataChanged, this, index(0, 0), index(rowCount(), Server::P_Max - 1), QVector<int>()));
    return pServer;
}
