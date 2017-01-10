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

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    m_pUi(new Ui::MainWindow),
    m_pServerModel(new ServerModel)
{
    m_pUi->setupUi(this);

    m_pUi->listServer->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_pUi->listServer->setModel(m_pServerModel);
    m_pUi->listServer->horizontalHeader()->setSectionResizeMode(Server::P_Name, QHeaderView::Stretch);
    m_pUi->listServer->horizontalHeader()->setSectionResizeMode(Server::P_ServerName, QHeaderView::Stretch);
    m_pUi->listServer->hideColumn(Server::P_BotsCurrent);
    m_pUi->listServer->hideColumn(Server::P_PlayerCurrent);
    m_pUi->listServer->hideColumn(Server::P_Description);
    m_pUi->listServer->hideColumn(Server::P_Port);
    m_pUi->listServer->hideColumn(Server::P_Url);
    m_pUi->listServer->horizontalHeader()->setVisible(true);

    QSettings s;
    s.beginGroup("Server");
    QStringList serverList = s.value("server_list").toStringList();

    int row = 0;
    for(const QVariant &server : serverList)
    {
        QString serverName = server.toString();
        m_ServerNames.insert(serverName);
        m_pServerModel->insertRow(row);
        m_pServerModel->setData(m_pServerModel->index(row, Server::P_Name), serverName, Qt::DisplayRole);
        m_pServerModel->setData(m_pServerModel->index(row, Server::P_Url), s.value(serverName + "_url").toString(), Qt::DisplayRole);
        m_pServerModel->setData(m_pServerModel->index(row, Server::P_Port), s.value(serverName + "_port").toInt(), Qt::DisplayRole);
    }
    s.endGroup();

    connect(m_pUi->buttonRemoveServer, &QPushButton::clicked, [this]()
    {
        QModelIndex index = m_pUi->listServer->selectionModel()->currentIndex();
        QString name = m_pServerModel->data(index, Qt::DisplayRole).toString();
        m_pServerModel->removeRow(index.row());
        m_ServerNames.remove(name);
        QSettings s;
        s.beginGroup("Server");
        s.setValue("server_list", QVariant(QStringList(m_ServerNames.toList())));
        s.remove(name + "_url");
        s.remove(name + "_port");
        s.endGroup();

        if(m_pUi->listServer->selectionModel()->selectedRows().empty())
            m_pUi->buttonRemoveServer->setEnabled(false);
    });

    connect(m_pUi->listServer, &QListWidget::clicked, [this]()
    {
        if(m_pUi->listServer->selectionModel()->selectedRows().empty())
            m_pUi->buttonRemoveServer->setEnabled(false);
        else
            m_pUi->buttonRemoveServer->setEnabled(true);
    });

    connect(m_pUi->buttonAddServer, &QPushButton::clicked, [this]()
    {
        DialogAddServer *pDialog = new DialogAddServer(this);
        connect(pDialog, &DialogAddServer::selected, [this](const QString &url, const QString &name, quint16 port)
        {
            if(m_ServerNames.contains(name))
            {
                QMessageBox::warning(this, tr("Error"), tr("Servername already in use"));
                return;
            }
            int row = m_pServerModel->rowCount();
            if(!m_pServerModel->insertRow(row))
                throw std::runtime_error("error");

            QSettings s;
            s.beginGroup("Server");
            QStringList  list(m_ServerNames.toList());
            list << name;
            s.setValue("server_list", list);
            s.setValue(name + "_url", url);
            s.setValue(name + "_port", port);
            s.endGroup();

            m_ServerNames.insert(name);
            m_pServerModel->setData(m_pServerModel->index(row, Server::P_Url), url, Qt::DisplayRole);
            m_pServerModel->setData(m_pServerModel->index(row, Server::P_Name), name, Qt::DisplayRole);
            m_pServerModel->setData(m_pServerModel->index(row, Server::P_Port), port, Qt::DisplayRole);
        });
        pDialog->setModal(true);
        pDialog->show();
//        m_pUi->listServer->addItem(m_pUi->editAddServer->text());
//        m_pUi->editAddServer->clear();
//        m_pUi->buttonAddServer->setEnabled(false);
    });
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}
