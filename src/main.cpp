// EXTERNAL INCLUDES
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>

// INTERNAL INCLUDES
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationDisplayName("Gothic Multiplayer Launcher");
    QApplication::setApplicationName("Gothic Multiplayer Launcher");
    QApplication::setApplicationVersion("v0.5.3");
    QApplication::setOrganizationName("Public domain");

    QApplication app(argc, argv);

    QPalette pal(QColor("#464754"));
	pal.setBrush(QPalette::WindowText, QColor("#D5C7A9"));
	pal.setBrush(QPalette::Button, QColor("#252433"));
	pal.setBrush(QPalette::Light, Qt::green);
	pal.setBrush(QPalette::Midlight, Qt::green);
	pal.setBrush(QPalette::Dark, Qt::green);
	pal.setBrush(QPalette::Mid, Qt::green);
	pal.setBrush(QPalette::Text, QColor("#D5C7A9"));
	pal.setBrush(QPalette::BrightText, QColor("#D5C7A9"));
	pal.setBrush(QPalette::ButtonText, QColor("#D5C7A9"));
	pal.setBrush(QPalette::Base, QColor("#464754"));
	pal.setBrush(QPalette::Window, QColor("#252433"));
	pal.setBrush(QPalette::Shadow, Qt::green);
	pal.setBrush(QPalette::Highlight, QColor("#555666"));
	pal.setBrush(QPalette::HighlightedText, Qt::green);
	pal.setBrush(QPalette::Link, Qt::green);
	pal.setBrush(QPalette::LinkVisited, Qt::green);
	pal.setBrush(QPalette::AlternateBase, QColor("#555666"));
	pal.setBrush(QPalette::NoRole, Qt::green);
	pal.setBrush(QPalette::ToolTipBase, QColor("#252433"));
	pal.setBrush(QPalette::ToolTipText, QColor("#D5C7A9"));
    app.setPalette(pal);

#ifdef _WIN32
	qApp->setStyle("fusion");
#endif

    MainWindow w;
    w.show();

    return app.exec();
}
