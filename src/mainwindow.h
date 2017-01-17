#pragma once

#include <QMainWindow>
#include <QString>
#include <QDataWidgetMapper>

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
    QDataWidgetMapper *m_pMapper;
};
