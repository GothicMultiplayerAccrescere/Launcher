#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::Options)
{
    m_pUi->setupUi(this);
}

Options::~Options()
{
    delete m_pUi;
}
