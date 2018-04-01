#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>

#include "dialogaddserver.h"
#include "ui_dialogaddserver.h"

DialogAddServer::DialogAddServer(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::DialogAddServer)
{
    m_pUi->setupUi(this);

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
