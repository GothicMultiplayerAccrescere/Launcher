#pragma once

#include <QDialog>
#include <QRegExpValidator>

namespace Ui {
    class DialogAddServer;
}

class DialogAddServer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddServer(QWidget *parent = nullptr);
    virtual ~DialogAddServer() override;

private:
    Ui::DialogAddServer *m_pUi;
    QRegExpValidator m_IPValidator;
    static QRegExp ipV4Regex;
    static QRegExp ipV6Regex;
    static QRegExp ipHostnameRegex;
    void checkRegexAndClearIfInvalid(QRegExp &validator);

signals:
    void selected(const QString &name, const QString &url, quint16 port);

private slots:
    void on_rbt_IPv4_clicked();
    void on_rbt_IPv6_clicked();
    void on_rbt_IPHostname_clicked();
};
