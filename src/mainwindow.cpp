#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QListWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

#include "dialogaddserver.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "servermodel.h"
#include "server.h"

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    m_pUi(new Ui::MainWindow),
    m_pServerModel(new ServerModel),
    m_pMapper(new QDataWidgetMapper(this))
{
    m_pUi->setupUi(this);

    m_pMapper->setModel(m_pServerModel);
    m_pMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    m_pMapper->addMapping(m_pUi->labelPort, Server::P_Port);
    m_pMapper->addMapping(m_pUi->labelUrl, Server::P_Url);
    m_pMapper->addMapping(m_pUi->serverDescription, Server::P_Description);
    m_pMapper->addMapping(m_pUi->nickname, Server::P_Nick);
    m_pMapper->addMapping(m_pUi->editAlias, Server::P_Name);

    m_pUi->listServer->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUi->listServer->setModel(m_pServerModel);
    m_pUi->listServer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pUi->listServer->hideColumn(Server::P_Description);
    m_pUi->listServer->hideColumn(Server::P_Port);
    m_pUi->listServer->hideColumn(Server::P_Url);
    m_pUi->listServer->hideColumn(Server::P_Nick);
    m_pUi->listServer->horizontalHeader()->setVisible(true);

    connect(m_pUi->buttonUpdateServerList, &QPushButton::clicked, m_pServerModel, &ServerModel::updateRecords);

    connect(m_pUi->buttonRemoveServer, &QPushButton::clicked, [this]()
    {
        QModelIndex index = m_pUi->listServer->selectionModel()->currentIndex();
        m_pServerModel->removeRow(index.row());
    });

    connect(m_pUi->listServer->selectionModel(), &QItemSelectionModel::selectionChanged, [this]()
    {
        m_pMapper->submit();
        if(m_pUi->listServer->selectionModel()->selectedRows().empty())
        {
            m_pUi->buttonJoin->setEnabled(false);
            m_pUi->buttonRemoveServer->setEnabled(false);
            m_pMapper->setCurrentModelIndex(QModelIndex());
            m_pUi->labelPort->clear();
            m_pUi->labelUrl->clear();
            m_pUi->serverDescription->clear();
            m_pUi->nickname->clear();
            m_pUi->editAlias->clear();
        }
        else
        {
            m_pUi->buttonJoin->setEnabled(true);
            m_pUi->buttonRemoveServer->setEnabled(true);
            m_pMapper->setCurrentIndex(m_pUi->listServer->selectionModel()->selectedRows().at(0).row());
        }
    });

    connect(m_pUi->buttonAddServer, &QPushButton::clicked, [this]()
    {
        DialogAddServer *pDialog = new DialogAddServer(this);
        connect(pDialog, &DialogAddServer::selected, m_pServerModel, &ServerModel::appendRecord);
        pDialog->setModal(true);
        pDialog->show();
    });
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}
