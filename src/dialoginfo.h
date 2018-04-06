#pragma once
#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfo(QWidget *pParent = nullptr);
    virtual ~DialogInfo() override;

private:
    Ui::DialogInfo *m_pUi;
    QPixmap m_Logo;
    QGraphicsScene *m_pScene;
};
