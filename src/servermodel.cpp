#include "servermodel.h"
#include "server.h"

ServerModel::ServerModel(QObject *pParent) :
    QAbstractTableModel(pParent)
{
    m_Timer.setInterval(1000);
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
    if(role != Qt::DisplayRole)
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

bool ServerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || index.parent().isValid() || index.row() >= m_Server.size() || index.column() >= Server::P_Max || role != Qt::DisplayRole)
        return false;

    Server *server = m_Server[index.row()];
    switch(index.column())
    {
    case Server::P_Name:
        server->setName(value.toString());
        return true;
    case Server::P_Url:
        server->setUrl(value.toString());
        return true;
    case Server::P_Port:
        server->setPort(value.toInt());
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
        server->setPintCurrent(value.toLongLong());
        return true;
    }

    return false;
}

bool ServerModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid() || row > m_Server.size())
        return false;

    beginInsertRows(parent, row, row + count - 1);
    Server *pServer = new Server;
    m_Server.insert(row, count, pServer);
    connect(&m_Timer, &QTimer::timeout, pServer, &Server::update);
    endInsertRows();

    return true;
}

bool ServerModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid() || row + count > m_Server.size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row, end = row + count; i < end; ++i)
        delete m_Server[i];

    m_Server.remove(row, count);
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

void ServerModel::startUpdates()
{
    m_Timer.start();
}

void ServerModel::updateRecords()
{
    for(Server *pServer : m_Server)
        pServer->update();
}
