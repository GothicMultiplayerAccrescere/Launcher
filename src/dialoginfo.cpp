#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
	ui->setupUi(this);

	logo = new QPixmap(":/logo/logo.png");
	scene = new QGraphicsScene(this);
	scene->addPixmap(*logo);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->show();
}

DialogInfo::~DialogInfo()
{
	delete scene;
	delete logo;
    delete ui;
}
