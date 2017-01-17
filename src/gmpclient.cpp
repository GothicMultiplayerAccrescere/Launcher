#include <QUdpSocket>
#include <QStringList>

#include "gmpclient.h"

// BUG: the protocol needs to be properly reversed, this hacky approach doesn't reliably work.

GMPClient::GMPClient(QObject *pParent) :
    QObject(pParent),
    m_pSocket(new QUdpSocket(this))
{
    connect(m_pSocket, &QUdpSocket::readyRead, this, &GMPClient::readyRead);
    connect(m_pSocket, &QUdpSocket::connected, this, &GMPClient::connected);
}

GMPClient::~GMPClient()
{
    if(m_pSocket->isOpen())
        m_pSocket->close();

    m_pSocket->deleteLater();
}

void GMPClient::start(const QString &address, quint16 port)
{
    if(m_pSocket->isOpen())
    {
        m_pSocket->disconnectFromHost();
        m_pSocket->close();
    }
    m_pSocket->connectToHost(address, port);
}

void GMPClient::readyRead()
{
    QByteArray buf = m_pSocket->readAll();
    switch(static_cast<unsigned char>(buf[0]))
    {
    case 6:
    {
        // Don't know, don't care
        unsigned char data[] = { 0x07, 0x00, 0xff, 0xff, 0x00, 0xfe, 0xfe, 0xfe,
                                 0xfe, 0xfd, 0xfd, 0xfd, 0xfd, 0x12, 0x34, 0x56,
                                 0x78, 0x04, 0xda, 0x87, 0x40, 0xab, 0x7d, 0x0f,
                                 0x04, 0xb0, 0x0e, 0xe0, 0x00, 0x03, 0x65, 0xff,
                                 0x74, 0x0d };
        m_pSocket->write(reinterpret_cast<char *>(data), sizeof(data));
        break;
    }
    case 8:
    {
        // Don't know, don't care
        unsigned char data[] = { 0x84, 0x00, 0x00, 0x00, 0x40, 0x01, 0x10, 0x00,
                                 0x00, 0x00, 0x09, 0x07, 0xf0, 0x00, 0x04, 0xbc,
                                 0xbc, 0x2b, 0xd4, 0x00, 0x00, 0x00, 0x00, 0x01,
                                 0x36, 0x7d, 0xf4, 0x00, 0x62, 0x35, 0x72, 0x36,
                                 0x6b, 0x51, 0x36, 0x67, 0x70, 0x30, 0x47, 0x63,
                                 0x70, 0x4b, 0x34, 0x78 };
        m_pSocket->write(reinterpret_cast<char *>(data), sizeof(data));
        break;
    }
    case 0x84:
    {
        if(static_cast<uint8_t>(buf[1]) < 2)
        {
            unsigned char data[] = { 0x84, 0x01, 0x00, 0x00, 0x60, 0x02, 0xf0, 0x01,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x04,
                                     0xda, 0x87, 0x40, 0xab, 0x7d, 0x0f, 0x04, 0x3f,
                                     0x57, 0xff, 0xd2, 0x00, 0x00, 0x04, 0xff, 0xff,
                                     0xff, 0xff, 0x00, 0x00, 0x04, 0xff, 0xff, 0xff,
                                     0xff, 0x00, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff,
                                     0x00, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0x00,
                                     0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
                                     0x04, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x04,
                                     0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x04, 0xff,
                                     0xff, 0xff, 0xff, 0x00, 0x00, 0x04, 0xff, 0xff,
                                     0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x20, 0x67, 0x0b, 0x28, 0x00, 0x00, 0x00, 0x00,
                                     0x01, 0x36, 0x7e, 0x0c, 0x00, 0x00, 0x48, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x01, 0x36, 0x7e, 0x0c,
                                     0x00, 0x00, 0x08, 0xb9 };
            m_pSocket->write(reinterpret_cast<char *>(data), sizeof(data));
        }
        else if(static_cast<uint8_t>(buf[1]) == 2)
        {
            const char *pStringData = reinterpret_cast<const char *>(buf.data());
            int32_t offset = 17;

            if(buf.size() < offset + 1)
                return;

            uint8_t length = static_cast<uint8_t>(buf[16]);

            if(buf.size() < offset + length + 2)
                return;

            QString serverName = QString::fromUtf8(pStringData + offset, length);
            offset += length + 2;
            length = static_cast<uint8_t>(buf[offset - 1]);

            if(buf.size() < offset + length + 2)
                return;

            QString gamemode = QString::fromUtf8(pStringData + offset, length);
            offset += length + 2;
            length = static_cast<uint8_t>(buf[offset - 1]);

            if(buf.size() < offset + length + 2)
                return;

            QString version = QString::fromUtf8(pStringData + offset, length);
            offset += length + 2;
            length = static_cast<uint8_t>(buf[offset - 1]);

            if(buf.size() < offset + length + 2)
                return;

            QString player = QString::fromUtf8(pStringData + offset, length);
            offset += length + 2;
            length = static_cast<uint8_t>(buf[offset - 1]);

            if(buf.size() < offset + length + 2)
                return;

            QString bots = QString::fromUtf8(pStringData + offset, length);
            offset += length + 2;
            length = static_cast<uint8_t>(buf[offset - 1]);

            if(buf.size() < offset + length + 2)
                return;

            QString description = QString::fromUtf8(pStringData + offset, length);
            m_pSocket->disconnectFromHost();
            m_pSocket->close();

            emit serverChecked(serverName, gamemode, version, player, bots, description);
        }
        break;
    }
    case 0xC0:
    {
        // seems like a keepalive
        break;
    }
    }
}

void GMPClient::connected()
{
    unsigned char data[1172] = { 0 };
    data[0] = 0x05;
    data[1] = 0x00;
    data[2] = 0xff;
    data[3] = 0xff;
    data[4] = 0x00;
    data[5] = 0xfe;
    data[6] = 0xfe;
    data[7] = 0xfe;
    data[8] = 0xfe;
    data[9] = 0xfd;
    data[10] = 0xfd;
    data[11] = 0xfd;
    data[12] = 0xfd;
    data[13] = 0x12;
    data[14] = 0x34;
    data[15] = 0x56;
    data[16] = 0x78;
    data[17] = 0x05;
    //const unsigned char data[] = {0x84, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x03, 0x00, 0x00, 0x00, 0x00, 0x20, 0x0f, 0x4c, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0xc0, 0x76};
    m_pSocket->write(reinterpret_cast<const char *>(data), sizeof(data));
}
