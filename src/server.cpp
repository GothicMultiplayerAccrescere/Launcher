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
    bool up = false;
    if(m_ServerName != serverName)
    {
        up = true;
        m_ServerName = serverName;
    }

    if(m_GameMode != gamemode)
    {
        up = true;
        m_GameMode = gamemode;
    }

    if(m_ServerVersion != version)
    {
        up = true;
        m_ServerVersion = version;
    }

    if(m_PlayerCount != player)
    {
        up = true;
        m_PlayerCount = player;
    }

    if(m_BotCount != bots)
    {
        up = true;
        m_BotCount = bots;
    }

    if(m_Description != description)
    {
        up = true;
        m_Description = description;
    }

    if(!up)
        return;

    emit updated();
}
