#pragma once

#include <QMainWindow>
#include <QString>

class QDataWidgetMapper;
class QProcess;
class ServerModel;
class Ticker;

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
    void setLineEditsEnabled(bool enabled);

    Ui::MainWindow *m_pUi;
    ServerModel *m_pServerModel;
    QDataWidgetMapper *m_pMapper;
};
