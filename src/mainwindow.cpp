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
#include <QDataWidgetMapper>
#include <QProcess>
#include <QDebug>

#include "dialogaddserver.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "servermodel.h"
#include "server.h"

#ifndef __unix__
#include <windows.h>
#endif

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    m_pUi(new Ui::MainWindow),
    m_pServerModel(new ServerModel),
    m_pMapper(new QDataWidgetMapper(this)),
    m_pGameProcess(nullptr)
{
    m_pUi->setupUi(this);

    m_pMapper->setModel(m_pServerModel);
    m_pMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    m_pUi->listServer->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pUi->listServer->setModel(m_pServerModel);
    m_pUi->listServer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pUi->listServer->hideColumn(Server::P_Description);
    m_pUi->listServer->hideColumn(Server::P_Port);
    m_pUi->listServer->hideColumn(Server::P_Url);
    m_pUi->listServer->hideColumn(Server::P_Nick);
    m_pUi->listServer->horizontalHeader()->setVisible(true);

    connect(m_pUi->buttonJoin, &QPushButton::clicked, this, &MainWindow::startProcess);

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
            m_pMapper->clearMapping();
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
            m_pMapper->addMapping(m_pUi->labelPort, Server::P_Port);
            m_pMapper->addMapping(m_pUi->labelUrl, Server::P_Url);
            m_pMapper->addMapping(m_pUi->serverDescription, Server::P_Description);
            m_pMapper->addMapping(m_pUi->nickname, Server::P_Nick);
            m_pMapper->addMapping(m_pUi->editAlias, Server::P_Name);
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
    delete m_pGameProcess;
}

void MainWindow::startProcess()
{
    QSettings s;
    s.beginGroup("Gothic");
    QString workingDir = s.value("working_directory").toString();
    s.endGroup();

    int row = m_pUi->listServer->selectionModel()->selectedRows().at(0).row();
    QString url = m_pServerModel->data(m_pServerModel->index(row, Server::P_Url), Qt::DisplayRole).toString();
    quint16 port = static_cast<uint16_t>(m_pServerModel->data(m_pServerModel->index(row, Server::P_Port), Qt::DisplayRole).toInt());
    QString nick = m_pServerModel->data(m_pServerModel->index(row, Server::P_Nick), Qt::DisplayRole).toString();

    // format of gmp_connect.cfg
    // nickname=Nickname
    // ip=127.0.0.1
    // port=28960

    QString filename;
    if(!workingDir.isEmpty())
        filename = workingDir + "/gmp_connect.cfg";
    else
        filename = "gmp_connect.cfg";

    QFile connectConf(filename);
    if(!connectConf.open(QFile::WriteOnly))
    {
        QMessageBox::critical(this, "Error", "Could not open gmp_connect.cfg");
        return;
    }

    connectConf.write(("nickname=" + nick + "\nip=" + url + "\nport=" + QString::number(port)).toLatin1());

    connectConf.close();

#ifdef __unix__
    s.beginGroup("Linux");
    if(m_pGameProcess)
    {
        QMessageBox::warning(this, "Already running", "The game is already running");
        return;
    }
    m_pGameProcess = new QProcess(this);
    connect(m_pGameProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished), [this](int exitCode)
    {
        if(exitCode)
            QMessageBox::critical(this, "Error", "Gothic exited with error code");
        m_pGameProcess->deleteLater();
        m_pGameProcess = nullptr;
    });
    m_pGameProcess->setEnvironment(QStringList() << "TARGET_DLL=gmp.dll" << "JUMP_DLL=loader.dll");
    m_pGameProcess->setArguments(QStringList() << "gothic.exe");
    m_pGameProcess->setProgram(s.value("wine_binary", "wine").toString());
    m_pGameProcess->setWorkingDirectory(workingDir);
    m_pGameProcess->start();
    s.endGroup();
#else
    s.beginGroup("Windows");
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOA si = { 0 };
    si.cb = sizeof(si);

    //-zMaxFrameRate: -zlog: -zwindow -zreparse nomenu -vdfs:physicalfirst
    std::string name("Gothic2.exe");
    std::string args(name);

    args = args + " -session " + "ZNOEXHND";

    if (!CreateProcessA(name.c_str(), const_cast<char *>(args.c_str()), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
    {
        printf("error creating process\n");
        printf("arguments: %s\n", args.c_str());
        return;
    }

    DWORD old;
    const char *data = "hacker.dll";
    VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, PAGE_EXECUTE_READWRITE, &old);
    WriteProcessMemory(pi.hProcess, (void*)0x0088B6FC, data, strlen(data) + 1, NULL);
    VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, old, &old);

    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    s.endGroup();
#endif
}
