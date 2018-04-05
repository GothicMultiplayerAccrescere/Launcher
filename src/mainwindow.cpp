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
#include <QAction>
#include <QDebug>
#include <QMenu>

#include "dialogaddserver.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "servermodel.h"
#include "dialoginfo.h"
#include "server.h"
#include "options.h"

#ifndef __unix__
#include <windows.h>
#endif

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    m_pUi(new Ui::MainWindow),
    m_pServerModel(new ServerModel),
    m_pMapper(new QDataWidgetMapper(this))
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
    connect(m_pUi->listServer, &QTableView::doubleClicked, this, &MainWindow::startProcess);

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
            setLineEditsEnabled(false);
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
            setLineEditsEnabled(true);
        }
    });

    connect(m_pUi->buttonAddServer, &QPushButton::clicked, [this]()
    {
        DialogAddServer *pDialog = new DialogAddServer(this);
        connect(pDialog, &DialogAddServer::selected, m_pServerModel, &ServerModel::appendRecord);
        pDialog->setModal(true);
        pDialog->exec();
        delete pDialog;
    });

    connect(m_pUi->actionQuit, &QAction::triggered, []() { qApp->quit(); });
    connect(m_pUi->actionOptions, &QAction::triggered, []()
    {
        Options *pOptions = new Options;
        pOptions->setModal(true);
        pOptions->exec();
        delete pOptions;
    });
    connect(m_pUi->actionAbout, &QAction::triggered, []()
    {
        DialogInfo *pInfo = new DialogInfo;
        pInfo->show();
    });

    setLineEditsEnabled(false);
}

MainWindow::~MainWindow()
{
    delete m_pUi;
}

void MainWindow::startProcess()
{
    QModelIndexList index = m_pUi->listServer->selectionModel()->selectedRows();
    if (index.size() != 1)
        return;

    int row = index.front().row();
    qWarning() << "[Information]: Connecting to: " << m_pServerModel->data(m_pServerModel->index(row, Server::P_ServerName), Qt::DisplayRole).toString();

    QString launcherDir = QCoreApplication::applicationDirPath();

    QSettings s;
    s.beginGroup("gothic");
    QString workingDir = s.value("working_directory").toString();
    if (workingDir.isEmpty())
    {
        qWarning() << "[Warning]: Could not find Gothic directory. Using defaults.";
        workingDir = QCoreApplication::applicationDirPath();
    }

    workingDir += "/system";
    s.endGroup();

    QString url = m_pServerModel->data(m_pServerModel->index(row, Server::P_Url), Qt::DisplayRole).toString();
    uint16_t port = static_cast<uint16_t>(m_pServerModel->data(m_pServerModel->index(row, Server::P_Port), Qt::DisplayRole).toUInt());
    QString nick = m_pServerModel->data(m_pServerModel->index(row, Server::P_Nick), Qt::DisplayRole).toString();

    // format of gmp_connect.cfg
    // nickname=Nickname
    // ip=127.0.0.1
    // port=28960

    QString filename;
    std::string name;
    if (!workingDir.isEmpty())
    {
        filename = workingDir + "/gmp_connect.cfg";
        name = workingDir.toStdString() + "/" + s.value("gothic_binary", "Gothic2.exe").toString().toStdString();
    }
    else
    {
        filename = "gmp_connect.cfg";
        name = s.value("gothic_binary", "Gothic2.exe").toString().toStdString();
    }

    QFile connectConf(filename);
    if (!connectConf.open(QFile::WriteOnly))
    {
        //QMessageBox::critical(this, "Error", "Could not open " + filename);
        qWarning() << "[Information]: Could not open gmp config file. Ignoring...";
    }
    else
    {
        connectConf.write(("nickname=" + nick + "\nip=" + url + "\nport=" + QString::number(port)).toLatin1());
        connectConf.close();
    }

#ifndef __unix__
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOA si = { 0 };
    si.cb = sizeof(si);

    //-zMaxFrameRate: -zlog: -zwindow -zreparse nomenu -vdfs:physicalfirst
    std::string args(name + " -session ZNOEXHND");

    if (!CreateProcessA(name.c_str(), const_cast<char *>(args.c_str()), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, workingDir.toStdString().c_str(), &si, &pi))
    {
        qWarning() << "[Error]: Starting Gothic failed: (#" << GetLastError() << ")";
        qWarning() << "[Information]: Args: (" << args.c_str() << ")";
        qWarning() << "[Information]: Try running the launcher in admin mode and specify a valid start path.\n";
        return;
    }

    std::string dllPath = launcherDir.toStdString() + "/gmp-r10/gmp.dll";
    LPVOID hLLA = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    LPVOID hLib = VirtualAllocEx(pi.hProcess, NULL, dllPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(pi.hProcess, hLib, dllPath.c_str(), dllPath.length(), NULL);
    CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)hLLA, hLib, NULL, NULL);

    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
#endif
}

void MainWindow::setLineEditsEnabled(bool enabled)
{
    m_pUi->labelPort->setEnabled(enabled);
    m_pUi->labelUrl->setEnabled(enabled);
    m_pUi->editAlias->setEnabled(enabled);
    m_pUi->nickname->setEnabled(enabled);
}
