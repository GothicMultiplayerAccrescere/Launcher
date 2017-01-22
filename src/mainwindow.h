#pragma once

#include <QMainWindow>
#include <QString>

class QDataWidgetMapper;
class QProcess;
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

public slots:
    void startProcess();

private:
    Ui::MainWindow *m_pUi;
    ServerModel *m_pServerModel;
    QDataWidgetMapper *m_pMapper;
    QProcess *m_pGameProcess;
};
