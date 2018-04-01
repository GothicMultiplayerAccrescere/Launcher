#include <QStringList>
#include <QDebug>

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>

#include "gmpclient.h"

GMPClient::GMPClient(QObject *pParent) :
    QObject(pParent),
    m_pClient(nullptr)
{
    m_Timer.setInterval(10);
    connect(&m_Timer, &QTimer::timeout, this, &GMPClient::update);
}

GMPClient::~GMPClient()
{
    m_Timer.stop();
    RakNet::RakPeerInterface::DestroyInstance(m_pClient);
}

void GMPClient::start(const QString &address, quint16 port)
{
    if (m_pClient && m_Timer.isActive())
        return;

    if (m_Timer.isActive())
        m_Timer.stop();

    if (m_pClient)
        RakNet::RakPeerInterface::DestroyInstance(m_pClient);

    m_pClient = RakNet::RakPeerInterface::GetInstance();

    char password[] = "b5r6kQ6gp0GcpK4x";

    RakNet::SocketDescriptor sd;
    m_pClient->Startup(5, &sd, 1);
    RakNet::ConnectionAttemptResult result = m_pClient->Connect(address.toStdString().c_str(), port, password, sizeof(password) - 1);

    if (result != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
    {
        qWarning() << "could not connect to server: " << result;
        return;
    }

    m_Timer.start();
}

void GMPClient::update()
{
    RakNet::Packet *pPacket = m_pClient->Receive();
    if (!pPacket)
        return;

    switch (static_cast<uint8_t>(pPacket->data[0]))
    {
    case static_cast<uint8_t>(DefaultMessageIDTypes::ID_CONNECTION_REQUEST_ACCEPTED):
    {
        RakNet::BitStream stream;
        stream.Write(MessageIdentifiers::GET_SERVER_INFO);

        m_pClient->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, pPacket->systemAddress, 0);
        break;
    }
    case static_cast<uint8_t>(MessageIdentifiers::GET_SERVER_INFO):
    {
        ServerInfo info;
        size_t seek = 2;
        if (!info.deserialize(pPacket->data, pPacket->length, seek))
        {
            qWarning() << "invalid packet";
            return;
        }

        info.averagePing = m_pClient->GetAveragePing(pPacket->systemAddress);

        emit serverChecked(info);
        break;
    }
    case static_cast<uint8_t>(DefaultMessageIDTypes::ID_DISCONNECTION_NOTIFICATION):
    case static_cast<uint8_t>(DefaultMessageIDTypes::ID_REMOTE_DISCONNECTION_NOTIFICATION):
    {
        m_Timer.stop();
        RakNet::RakPeerInterface::DestroyInstance(m_pClient);
        m_pClient = nullptr;
        break;
    }
    }
}

bool readString(const uint8_t *pData, size_t maxlen, size_t &seek, QString &string)
{
    if (sizeof(uint8_t) + seek > maxlen)
        return false;

    uint8_t len = pData[seek];
    ++seek;
    if (len > maxlen - seek)
        return false;

    string = QString::fromUtf8(reinterpret_cast<const char *>(pData + seek), len);
    seek += len + 1;
    return true;
}

bool ServerInfo::deserialize(const uint8_t *pData, size_t maxlen, size_t &seek)
{
    if (!readString(pData, maxlen, seek, serverName))
        return false;

    if (!readString(pData, maxlen, seek, gamemode))
        return false;

    if (!readString(pData, maxlen, seek, version))
        return false;

    if (!readString(pData, maxlen, seek, player))
        return false;

    if (!readString(pData, maxlen, seek, bots))
        return false;

    if (!readString(pData, maxlen, seek, description))
        return false;

    return true;
}
