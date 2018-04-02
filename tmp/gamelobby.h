#pragma region Header
#pragma once

#include "Prerequisites.h"
#include "Ticker\ticker.h"
#pragma endregion

namespace GUI
{
	class GameLobby : public QFrame
	{
		//friend class WindowHandler;
		Q_OBJECT

	public:
		GameLobby();
		~GameLobby();

		inline bool existsFile(const std::string& name) {
			if (FILE *file = fopen(name.c_str(), "r")) {
				fclose(file);
				return true;
			}
			else {
				return false;
			}
		}
		QPixmap loadImage(QString, QString, QString);

	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	private:
		bool changeActiveWidget_(QWidget *_activate);
		void initHelp();
		void initUserList();
		GUI::ServerModel *m_pModel;
		//CustomModel::ServerTableModel *p_tableModel_;
		//CustomView::ServerTableView *p_tableView_;
		//CustomDelegate::ServerTableDelegate *p_tableDelegate_;
		//QListView *p_userListView_;
		//CustomModel::UserListModel *p_userListModel_;
		//CustomDelegate::UserListDelegate *p_userListDelegate_;
		//QTextBrowser *p_helpWidget_;
		QPoint m_DragPosition;
		bool m_MoveWindow;
		Ticker *m_pTicker;
		QWidget *m_pActiveWidget;
		bool m_BlockUserSend;
		bool m_IsTimerActive;
		Ui::GameLobby *m_pUi;

	signals:
		void getNewIcon(int, int);

	private slots:
		void serverSelectionChanged(QModelIndex);
		void serverSelectionChangedSelect(QItemSelection, QItemSelection);
		void on_minimizeButton_clicked();
		void on_closeButton_clicked();
		void on_moditorsCorner_clicked();
		void on_helpButton_clicked();
		//void serverRecordReceived(ServerRecordData);
		//void resourceReceived(ResourceData);
		//void userRecordReceived(Network::UserData);
		void searchTextEdited(QString);
		void research();
		void selectionChanged(int);

	public slots:
		void startGame(QModelIndex);
		void startGame(QString, quint16);

	};
}