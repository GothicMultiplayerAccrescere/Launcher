#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
	ui->setupUi(this);

	QPixmap* logo = new QPixmap(":/logo/logo.png");
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->addPixmap(*logo);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->show();
}

DialogInfo::~DialogInfo()
{
    delete ui;
}
