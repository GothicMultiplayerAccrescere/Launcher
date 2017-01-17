#include "server.h"

Server::Server() :
    m_PingCurrent(0),
    m_pClient(new GMPClient)
{
    connect(m_pClient, &GMPClient::serverChecked, this, &Server::updateData);
}

Server::~Server()
{
    delete m_pClient;
}

void Server::update()
{
    m_pClient->start(m_Url, m_Port);
}

void Server::updateData(const QString &serverName, const QString &gamemode, const QString &version, const QString &player, const QString &bots, const QString &description)
{
    m_ServerName = serverName;
    m_GameMode = gamemode;
    m_ServerVersion = version;
    m_PlayerCount = player;
    m_BotCount = bots;
    m_Description = description;

    emit updated();
}
