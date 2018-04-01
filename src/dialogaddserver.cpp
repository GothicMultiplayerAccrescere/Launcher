#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QRegExp>

#include "dialogaddserver.h"
#include "ui_dialogaddserver.h"

QRegExp DialogAddServer::ipV4Regex          = QRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
QRegExp DialogAddServer::ipV6Regex          = QRegExp("^([0-9a-f]){1,4}(:([0-9a-f]){1,4}){7}$");
QRegExp DialogAddServer::ipHostnameRegex    = QRegExp("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");

DialogAddServer::DialogAddServer(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::DialogAddServer),
    m_IPValidator(ipV4Regex, this)
{
    m_pUi->setupUi(this);
    m_pUi->editUrl->setValidator(&m_IPValidator);

    connect(m_pUi->buttonAddServer, &QPushButton::clicked, [this]()
    {
        emit selected(m_pUi->editServerName->text(), m_pUi->editUrl->text(), static_cast<quint16>(m_pUi->editPort->value()));
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

void DialogAddServer::checkRegexAndClearIfInvalid(QRegExp &regex)
{
    if(!regex.exactMatch(m_pUi->editUrl->text()))
        m_pUi->editUrl->clear();
}

void DialogAddServer::on_rbt_IPv4_clicked()
{
    m_pUi->rbt_IPv6->setChecked(false);
    m_pUi->rbt_IPHostname->setChecked(false);
    m_IPValidator.setRegExp(ipV4Regex);
    checkRegexAndClearIfInvalid(ipV4Regex);
}

void DialogAddServer::on_rbt_IPv6_clicked()
{
    m_pUi->rbt_IPv4->setChecked(false);
    m_pUi->rbt_IPHostname->setChecked(false);
    m_IPValidator.setRegExp(ipV6Regex);
    checkRegexAndClearIfInvalid(ipV6Regex);
}

void DialogAddServer::on_rbt_IPHostname_clicked()
{
    m_pUi->rbt_IPv4->setChecked(false);
    m_pUi->rbt_IPv6->setChecked(false);
    m_IPValidator.setRegExp(ipHostnameRegex);
    checkRegexAndClearIfInvalid(ipHostnameRegex);
}
