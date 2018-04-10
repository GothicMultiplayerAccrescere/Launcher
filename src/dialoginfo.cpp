#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include "resource.h"

DialogInfo::DialogInfo(QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::DialogInfo),
    m_Logo(":/logo/logo.png")
{
	m_pUi->setupUi(this);

	m_pScene = new QGraphicsScene(this);
    m_pScene->addPixmap(m_Logo);
    m_pUi->graphicsView->setScene(m_pScene);

    QString version(GIT_TAG);
    if (version.isEmpty())
        version = "dev";

    version = version + "-" + GIT_COMMIT;
    m_pUi->labelVersion->setText(version);
    m_pUi->labelBuildDate->setText(GIT_DATE);
}

DialogInfo::~DialogInfo()
{
    delete m_pScene;
    delete m_pUi;
}
