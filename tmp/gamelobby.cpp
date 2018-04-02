#pragma region Header
#include "prerequisites.h"
#pragma endregion

GUI::GameLobby *GUI::WindowHandler::s_pLobby = nullptr;

GUI::GameLobby::GameLobby() :
QFrame(nullptr),
m_pModel(new GUI::ServerModel),
//p_tableModel_(new CustomModel::ServerTableModel),
//p_tableView_(new CustomView::ServerTableView(this)),
//p_tableDelegate_(new CustomDelegate::ServerTableDelegate(p_tableView_, this)),
//p_userListView_(new QListView(this)),
//p_userListModel_(new CustomModel::UserListModel),
//p_userListDelegate_(new CustomDelegate::UserListDelegate),
//p_helpWidget_(new QTextBrowser(this)),
m_MoveWindow(false),
m_pTicker(new Ticker(this)),
m_pActiveWidget(nullptr),
m_BlockUserSend(false),
m_IsTimerActive(false),
m_pUi(new Ui::GameLobby)
{
	//connect(p_networkHandler_, SIGNAL(serverRecordReceived(ServerRecordData)), SLOT(serverRecordReceived(ServerRecordData)));
	//connect(p_networkHandler_, SIGNAL(resourceReceived(ResourceData)), SLOT(resourceReceived(ResourceData)));
	//connect(p_networkHandler_, SIGNAL(userRecordReceived(Network::UserData)), SLOT(userRecordReceived(Network::UserData)));
	//connect(p_tableDelegate_, SIGNAL(startGame(QModelIndex)), SLOT(startGame(QModelIndex)));

	m_pUi->setupUi(this);
	//connect(m_pUi->searchLine, SIGNAL(textEdited(QString)), SLOT(searchTextEdited(QString)));
	//connect(m_pUi->filterSelection, SIGNAL(currentIndexChanged(int)), SLOT(selectionChanged(int)));
	//connect(m_pUi->orderSelection, SIGNAL(currentIndexChanged(int)), SLOT(selectionChanged(int)));
	//connect(m_pUi->orderTypeSelection, SIGNAL(currentIndexChanged(int)), SLOT(selectionChanged(int)));
	m_pUi->userList->hide();
	setWindowFlags(Qt::FramelessWindowHint);

	m_pUi->verticalLayout->addWidget(m_pTicker);

	m_pUi->serverList->setItemDelegate(new GUI::ServerDelegate);
	m_pUi->serverList->setModel(m_pModel);

	//m_pUi->contentWidget->layout()->addWidget(p_tableView_);
	//p_tableView_->hide();
	//p_tableView_->setItemDelegate(p_tableDelegate_);
	//p_tableView_->setModel(p_tableModel_);

	QScroller::scroller(m_pUi->serverList);
	QScroller::grabGesture(m_pUi->serverList, QScroller::LeftMouseButtonGesture);
	//connect(m_pUi->serverList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(serverSelectionChangedSelect(QItemSelection, QItemSelection)));
	//connect(m_pUi->serverList, SIGNAL(clicked(QModelIndex)), SLOT(serverSelectionChanged(QModelIndex)));

	//m_pUi->serverList->setCurrentIndex(p_model_->index(0));

	initHelp();
	initUserList();

	//Network::ServerData singleplayer(true);
	//singleplayer.m_icon = QPixmap(":/controls/gothic.png");
	//singleplayer.m_name = "Singleplayer";
	//p_model_->append(singleplayer);

	Misc::ServerData server;
	server.m_icon = QPixmap(":/GameLobby/Resources/icon.png");
	server.m_name = "Server";
	m_pModel->append(server);
	Misc::ServerData server2;
	server2.m_icon = QPixmap(":/GameLobby/Resources/icon.png");
	server2.m_name = "Server2";
	m_pModel->append(server2);
	Misc::ServerData server3;
	server3.m_icon = QPixmap(":/GameLobby/Resources/icon.png");
	server3.m_name = "Server3";
	m_pModel->append(server3);
	Misc::ServerData server4;
	server4.m_icon = QPixmap(":/GameLobby/Resources/icon.png");
	server4.m_name = "Server4";
	m_pModel->append(server4);
	//QString string = QString::fromLatin1("<center><h1 style=\"font-family:arial; color:white;\">Überschrift</h1><br>Beispieltext</center>");
	//server.p_tableModel->insert(0, ServerTableRecord((int)Network::ResourceType::ID_SERVER_DESCRIPTION, QVariant::fromValue(string)));
	//server.p_tableModel->insert(1, ServerTableRecord((int)Network::ResourceType::ID_PLAY_GAME_OVERLAY, QVariant::fromValue<QPixmap>(QPixmap(":/controls/hover.png"))));

	//QDir::setCurrent(getGothicPath());
}

GUI::GameLobby::~GameLobby()
{
	//delete p_model_;
	//delete p_tableModel_;
	//delete p_tableView_;
	delete m_pUi;
	delete m_pTicker;
	//delete p_userListView_;
	//delete p_userListModel_;
	//delete p_userListDelegate_;
}

bool GUI::GameLobby::changeActiveWidget_(QWidget *_activate)
{
	if (_activate == m_pActiveWidget)
		return false;
	if (m_pActiveWidget != nullptr)
		m_pActiveWidget->hide();

	if (_activate != nullptr)
		_activate->show();
	else
		return false;
	return true;
}

void GUI::GameLobby::serverSelectionChangedSelect(QItemSelection selection1, QItemSelection selection2)
{
	if (selection1.indexes().size() == 1)
		serverSelectionChanged(selection1.indexes().at(0));
}

void GUI::GameLobby::serverSelectionChanged(QModelIndex _index)
{
	//changeActiveWidget_(p_tableView_);
	//m_pActiveWidget = p_tableView_;
	//CustomModel::ServerTableModel *stm = nullptr;
	//stm = p_model_->getModel(_index);
	//if (stm == nullptr)
	//	return;
	//p_tableView_->setModel(stm);
	//p_tableView_->resizeColumnsToContents();
	//p_tableView_->resizeRowsToContents();
}

//void GUI::GameLobby::serverRecordReceived(ServerRecordData serverData)
//{
//	Network::ServerData server;
//	QString path(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/data/server/" + QString::number(serverData.first) + "/");
//	QString file("icon");
//	QPixmap icon = loadImage(path, file, serverData.second.second);
//	if (icon.isNull())
//	{
//		RakNet::BitStream bitStream;
//		bitStream.Write((RakNet::MessageID)Network::PacketEnum::ID_RESOURCE);
//		bitStream.Write(QString::number(serverData.first).toStdString().c_str());
//		bitStream.Write(QString::number((int)Network::ResourceType::ID_SERVER_ICON).toStdString().c_str());
//		p_networkHandler_->p_client->Send(&bitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
//	}
//	else
//		server.m_icon = icon;
//
//	server.m_name = serverData.second.first;
//	server.m_id = serverData.first;
//	//server.p_tableModel = new CustomModel::ServerTableModel;
//	p_model_->append(server);
//}

QPixmap GUI::GameLobby::loadImage(QString path, QString file, QString hash = QString())
{
	if (!QDir().exists(path))
	{
		QDir().mkpath(path);
		return QPixmap();
	}
	QFile imageFile(path + file);
	if (!imageFile.open(QIODevice::ReadOnly))
	{
		return QPixmap();
	}

	QByteArray picture = QByteArray::fromBase64(imageFile.readAll());
	imageFile.close();

	if (hash != QString())
	{
		if (QString(QCryptographicHash::hash(picture, QCryptographicHash::Md5).toHex()) != hash)
			return QPixmap();
	}

	QPixmap icon;
	if (!icon.loadFromData(picture, "png"))
		return QPixmap();
	return icon;
}

//void GUI::GameLobby::resourceReceived(ResourceData resource)
//{
//	p_model_->setImage(resource);
//}

void GUI::GameLobby::startGame(QModelIndex _index)
{
	//if (m_pUi->serverList->currentIndex().row() == 0 && 0)
	//{
	//	QString gothicPath;
	//	QString modName;// = p_model_->m_serverlist_.at(m_pUi->serverList->currentIndex().row()).m_modString;
	//	//if (!QDir().rename(gothicPath + "\\Data\\modvdf\\" + modName, gothicPath + "\\Data\\" + modName))
	//	//{
	//	//	QMessageBox::critical(this, "Error", "Couldn't start Mod", NULL, NULL);
	//	//	return;
	//	//}
	//	QString modArg;// = p_model_->m_serverlist_.at(m_pUi->serverList->currentIndex().row()).m_iniList.at(_index.row());
	//	QStringList argList("-game:" + modArg);
	//	argList.append("-znomusic");
	//	argList.append("-znosound");
	//	QProcess::startDetached("\"" + gothicPath + "\\system\\Gothic2.exe\"", argList, "/system/");
	//}

	//Execute hack
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFOA si = { 0 };
	si.cb = sizeof(si);

	QString game = "\\system\\Gothic2.exe";
	//-zMaxFrameRate: -zlog: -zwindow -zreparse nomenu -vdfs:physicalfirst
	QString args(QString("Gothic2.exe host=") + "arcanumgames.servegame.com" + " port=" + "2001" + " -session " + "sessionId" + " -banner " + "ZNOEXHND"/* + " -nomenu"*/);

	if (!existsFile("Gothic2.exe"))
	{
		QMessageBox::critical(this, "File not found", "File not found: " + QString("Gothic2.exe"));
		QMessageBox::critical(this, "File not found", "File not found: " + QString("Gothic2.exe"));
		QMessageBox::critical(this, "File not found", "File not found: " + QString("Gothic2.exe"));
		QMessageBox::critical(this, "File not found", "File not found: " + QString("Gothic2.exe"));
		return;
	}

	if (!CreateProcessA("Gothic2.exe", (char*)args.toStdString().c_str(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		QMessageBox::critical(this, "Injection aborded", QString("An error occured during injection.\r\nCode: %1").arg(GetLastError()));
		return;
	}

	DWORD old;
	const char *data = "client.dll";
	VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, PAGE_EXECUTE_READWRITE, &old);
	WriteProcessMemory(pi.hProcess, (void*)0x0088B6FC, data, strlen(data) + 1, NULL);
	VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, old, &old);

	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void GUI::GameLobby::startGame(QString _host, quint16 _port)
{
	QSettings settings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/GameLobby.ini", QSettings::IniFormat);
	settings.beginGroup("Paths");
	QString gothicDir(settings.value("GothicDir").toString());
	settings.endGroup();
	QDir::setCurrent(gothicDir);
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFOA si = { 0 };
	si.cb = sizeof(si);

	//-zMaxFrameRate: -zlog: -zwindow -zreparse nomenu -vdfs:physicalfirst
	QString args("Gothic2.exe host=" + _host + " port=" + QString::number(_port) + " -session " + "sessionId" + " -banner " + "ZNOEXHND"/* + " -nomenu"*/);

	if (!existsFile("Gothic2.exe"))
	{
		QMessageBox::critical(this, "Error", "File not found: " + gothicDir + "Gothic2.exe");
		return;
	}

	if (!CreateProcessA("Gothic2.exe", (char*)args.toStdString().c_str(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		QMessageBox::critical(this, "Injection aborded", QString("An error occured during injection.\r\nCode: %1").arg(GetLastError()));
		return;
	}

	DWORD old;
	const char *data = "client.dll";
	VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, PAGE_EXECUTE_READWRITE, &old);
	WriteProcessMemory(pi.hProcess, (void*)0x0088B6FC, data, strlen(data) + 1, NULL);
	VirtualProtectEx(pi.hProcess, (void*)0x0088B6FC, strlen(data) + 1, old, &old);

	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	//QDir::setCurrent(QCoreApplication::applicationFilePath());
}

#pragma region DragWindow
void GUI::GameLobby::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && event->y() < 75 && !(event->y() == 0 && event->x() == 0))
	{
		m_MoveWindow = true;
		m_DragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void GUI::GameLobby::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_MoveWindow = false;
	}
}

void GUI::GameLobby::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton && m_MoveWindow)
	{
		move(event->globalPos() - m_DragPosition);
		event->accept();
	}
}
#pragma endregion

#pragma region ButtonSlots
void GUI::GameLobby::on_minimizeButton_clicked()
{
	showMinimized();
}

void GUI::GameLobby::on_closeButton_clicked()
{
	//p_networkHandler_->disconnect();
	//p_networkHandler_->m_STOP = true;
	//p_networkHandler_->wait(200);
	close();
}

void GUI::GameLobby::on_moditorsCorner_clicked()
{
	changeActiveWidget_(nullptr);
	m_pActiveWidget = nullptr;
	startGame("178.4.17.145", 2001);
}

void GUI::GameLobby::on_helpButton_clicked()
{
	//changeActiveWidget_(p_helpWidget_);
	//m_pActiveWidget = p_helpWidget_;
}
#pragma endregion

#pragma region WidgetInitialisation
void GUI::GameLobby::initHelp()
{
	//	p_helpWidget_->hide();
	//	p_helpWidget_->setStyleSheet("QTextBrowser {background-color:rgba(20, 20, 20, 130); border:none; color:white; padding-right:1px;} QScrollBar { border: 1px solid black; background-color:#108738B4; width: 20px;}");
	//	m_pUi->contentWidget->layout()->addWidget(p_helpWidget_);
}

void GUI::GameLobby::initUserList()
{
	//	m_pUi->userList->layout()->addWidget(p_userListView_);
	//	p_userListView_->setStyleSheet("QListView{ background-color:transparent; border:none;}");
	//	p_userListView_->setItemDelegate(p_userListDelegate_);
	//	p_userListView_->setModel(p_userListModel_);
	//	p_userListView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//	p_userListView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//	p_userListView_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	//	p_userListView_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//
	//	m_pScroller = QScroller::scroller(p_userListView_);
	//	QScroller::grabGesture(p_userListView_, QScroller::LeftMouseButtonGesture);
}
#pragma endregion

#pragma region Searchbar
void GUI::GameLobby::searchTextEdited(QString _searchRequest)
{
	//	if (!m_BlockUserSend)
	//	{
	//		m_BlockUserSend = true;
	//		p_userListModel_->clear();
	//		RakNet::BitStream bitStream;
	//		bitStream.Write((RakNet::MessageID)Network::PacketEnum::ID_USERLIST);
	//		bitStream.Write(RakNet::RakString(_searchRequest.toStdString().c_str()));
	//		bitStream.Write("0");
	//		bitStream.Write("30");
	//		bitStream.Write(RakNet::RakString(std::to_string(m_pUi->orderSelection->currentIndex()).c_str()));
	//		m_pUi->orderTypeSelection->currentIndex() == 0 ? bitStream.Write0() : bitStream.Write1();
	//		bitStream.Write(RakNet::RakString(std::to_string(m_pUi->filterSelection->currentIndex()).c_str()));
	//		p_networkHandler_->p_client->Send(&bitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	//	}
	//	else
	//	{
	//		if (!m_IsTimerActive && p_networkHandler_->isConnected())
	//		{
	//			m_IsTimerActive = true;
	//			QTimer::singleShot(500, this, SLOT(research()));
	//		}
	//	}
}

//void GUI::GameLobby::userRecordReceived(Network::UserData data)
//{
//	if (data.isEmpty())
//	{
//		m_BlockUserSend = false;
//		return;
//	}
//	p_userListModel_->append(data);
//}

void GUI::GameLobby::research()
{
	Sleep(100);
	m_IsTimerActive = false;
	//m_BlockUserSend = false;
	emit searchTextEdited(m_pUi->searchLine->text());
}

void GUI::GameLobby::selectionChanged(int)
{
	research();
}
#pragma endregion
