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
            setLineEditsEnabled(false);
            m_pMapper->clearMapping();
            m_pMapper->setCurrentModelIndex(QModelIndex());
            m_pUi->labelPort->clear();
            m_pUi->labelUrl->clear();
            m_pUi->serverDescription->clear();
            m_pUi->nickname->clear();
            m_pUi->editAlias->clear();
        }
        else
        {
            setLineEditsEnabled(true);
            m_pMapper->addMapping(m_pUi->labelPort, Server::P_Port);
            m_pMapper->addMapping(m_pUi->labelUrl, Server::P_Url);
            m_pMapper->addMapping(m_pUi->serverDescription, Server::P_Description);
            m_pMapper->addMapping(m_pUi->nickname, Server::P_Nick);
            m_pMapper->addMapping(m_pUi->editAlias, Server::P_Name);
            m_pMapper->setCurrentIndex(m_pUi->listServer->selectionModel()->selectedRows().at(0).row());
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
        pInfo->exec();
        delete pInfo;
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
    QString name;
    if (!workingDir.isEmpty())
    {
        filename = workingDir + "/gmp_connect.cfg";
        name = workingDir + "/" + s.value("gothic_binary", "Gothic2.exe").toString();
    }
    else
    {
        filename = "gmp_connect.cfg";
        name = s.value("gothic_binary", "Gothic2.exe").toString();
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

    //-zMaxFrameRate: -zlog: -zwindow -zreparse nomenu -vdfs:physicalfirst
    QString args(name + " -session ZNOEXHND");
    wchar_t *wcharName = new wchar_t[name.length() + 1];
    memset(wcharName, 0, static_cast<size_t>(name.length() + 1));
    name.toWCharArray(wcharName);

    wchar_t *wcharArgs = new wchar_t[args.length() + 1];
    memset(wcharArgs, 0, static_cast<size_t>(args.length() + 1));
    args.toWCharArray(wcharArgs);

    wchar_t *wcharWorkingDirectory = new wchar_t[workingDir.length() + 1];
    memset(wcharWorkingDirectory, 0, static_cast<size_t>(workingDir.length() + 1));
    workingDir.toWCharArray(wcharWorkingDirectory);

    QString dllPath = launcherDir + "/gmp-r10/gmp.dll";
    size_t dllPathSize = static_cast<size_t>(dllPath.length() + 1);
    wchar_t *wcharDllPath = new wchar_t[dllPathSize];
    memset(wcharDllPath, 0, dllPathSize);
    dllPath.toWCharArray(wcharDllPath);
    dllPathSize *= 2;

#ifndef __unix__
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFOA si = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcessW(wcharName, wcharArgs, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, wcharWorkingDirectory, &si, &pi))
    {
        qWarning() << "[Error]: Starting Gothic failed: (#" << GetLastError() << ")";
        qWarning() << "[Information]: Args: (" << args << ")";
        qWarning() << "[Information]: Try running the launcher in admin mode and specify a valid start path.\n";
        return;
    }

    LPVOID hLLA = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    LPVOID hLib = VirtualAllocEx(pi.hProcess, NULL, dllPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(pi.hProcess, hLib, reinterpret_cast<char *>(wcharDllPath), dllPathSize, NULL);
    CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)hLLA, hLib, NULL, NULL);

    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
#endif

    delete []wcharName;
    delete []wcharArgs;
    delete []wcharWorkingDirectory;
    delete []wcharDllPath;
}

void MainWindow::setLineEditsEnabled(bool enabled)
{
    m_pUi->buttonJoin->setEnabled(enabled);
    m_pUi->buttonRemoveServer->setEnabled(enabled);
    m_pUi->labelPort->setEnabled(enabled);
    m_pUi->labelUrl->setEnabled(enabled);
    m_pUi->editAlias->setEnabled(enabled);
    m_pUi->nickname->setEnabled(enabled);
}
