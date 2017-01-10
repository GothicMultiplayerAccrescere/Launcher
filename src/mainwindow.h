#pragma once

#include <QMainWindow>
#include <QHash>
#include <QString>

class ServerModel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
public:
    MainWindow();
    virtual ~MainWindow() override;

private:
    Ui::MainWindow *m_pUi;
    ServerModel *m_pServerModel;
};
