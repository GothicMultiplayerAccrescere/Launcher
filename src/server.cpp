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

void Server::updateData(const ServerInfo &info)
{
    m_ServerName = info.serverName;
    m_GameMode = info.gamemode;
    m_ServerVersion = info.version;
    m_PlayerCount = info.player;
    m_BotCount = info.bots;
    m_Description = info.description;
    m_PingCurrent = info.averagePing;

    emit updated();
}
