// EXTERNAL INCLUDES
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>

// INTERNAL INCLUDES
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationDisplayName("Gothic Launcher");
    QApplication::setApplicationName("GMP Launcher");
    QApplication::setApplicationVersion("v0.0.1");
    QApplication::setOrganizationName("Public domain");

    QApplication app(argc, argv);

    //TODO: some windows user needs to put in a fitting style please.
    if(QStyleFactory::keys().contains("Oxygen"))
        QApplication::setStyle(QStyleFactory::create("Oxygen"));
    else if(QStyleFactory::keys().contains("Fusion"))
        QApplication::setStyle(QStyleFactory::create("Windows"));
    else
    {
        QMessageBox::critical(nullptr, "Style not found", "Your platform does not provide an accepted style.");
        qFatal("Style not found");
    }

    QPalette pal(QColor("#464754"));
    pal.setBrush(QPalette::Window, QColor("#252433"));
    pal.setBrush(QPalette::WindowText, QColor("#D5C7A9"));
    pal.setBrush(QPalette::Base, QColor("#464754"));
    pal.setBrush(QPalette::AlternateBase, QColor("#555666"));
    pal.setBrush(QPalette::ToolTipBase, QColor("#000000"));
    pal.setBrush(QPalette::ToolTipText, Qt::white);
    pal.setBrush(QPalette::Text, QColor("#D5C7A9"));
    pal.setBrush(QPalette::Button, QColor("#252433"));
    pal.setBrush(QPalette::ButtonText, QColor("#D5C7A9"));
    pal.setBrush(QPalette::BrightText, Qt::white);
    pal.setBrush(QPalette::Highlight, Qt::gray);
    pal.setBrush(QPalette::HighlightedText, Qt::yellow);
    app.setPalette(pal);

    MainWindow w;
    w.show();

    return app.exec();
}
