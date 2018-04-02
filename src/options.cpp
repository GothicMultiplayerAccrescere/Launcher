#include <QSettings>
#include <QFileDialog>

#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::Options)
{
    m_pUi->setupUi(this);

    connect(m_pUi->buttonCancel, &QPushButton::clicked, this, &Options::close);
    connect(m_pUi->buttonApply, &QPushButton::clicked, this, &Options::save);

    connect(m_pUi->buttonOk, &QPushButton::clicked, [this]()
    {
        save();
        close();
    });

    connect(m_pUi->editGothicPath, &QLineEdit::textChanged, [this]()
    {
        m_pUi->buttonApply->setEnabled(true);
    });

    connect(m_pUi->buttonGothicPath, &QPushButton::clicked, [this]()
    {
        QFileDialog *pDialog = new QFileDialog(this);
        if (!m_pUi->editGothicPath->text().isEmpty())
            pDialog->setDirectory(m_pUi->editGothicPath->text());

        pDialog->setFileMode(QFileDialog::Directory);
        pDialog->exec();
        QDir dir = pDialog->directory();
        m_pUi->editGothicPath->setText(dir.path());
        delete pDialog;
    });

    QSettings s;
    s.beginGroup("gothic");
    m_pUi->editGothicPath->setText(s.value("working_directory").toString());
    s.endGroup();

    m_pUi->buttonApply->setEnabled(false);
}

Options::~Options()
{
    delete m_pUi;
}

void Options::save()
{
    m_pUi->buttonApply->setEnabled(false);
    QSettings s;
    s.beginGroup("gothic");
    s.setValue("working_directory", m_pUi->editGothicPath->text());
    s.endGroup();
}
