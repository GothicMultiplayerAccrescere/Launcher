#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QRegExp>

#include "dialogaddserver.h"
#include "ui_dialogaddserver.h"

QRegExp DialogAddServer::ipV4Regex = QRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

DialogAddServer::DialogAddServer(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::DialogAddServer),
    m_pIPv4Validator(ipV4Regex, this)
{
    m_pUi->setupUi(this);
    m_pUi->editUrl->setValidator(&m_pIPv4Validator);

    connect(m_pUi->buttonAddServer, &QPushButton::clicked, [this]()
    {
        emit selected(m_pUi->editServerName->text(), m_pUi->editUrl->text(), m_pUi->editPort->value());
        close();
    });

    connect(m_pUi->buttonCancel, &QPushButton::clicked, [this]()
    {
        close();
    });
}

DialogAddServer::~DialogAddServer()
{
    delete m_pUi;
}
