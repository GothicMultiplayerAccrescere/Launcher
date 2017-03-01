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

    QPalette pal(Qt::black);
    pal.setBrush(QPalette::Window, QColor(0x25, 0x24, 0x33));
    pal.setBrush(QPalette::WindowText, QColor(0xd5, 0xc7, 0xa9));
    pal.setBrush(QPalette::Base, QColor(0x46, 0x47, 0x54));
    pal.setBrush(QPalette::AlternateBase, QColor(0x55, 0x56, 0x66));
    pal.setBrush(QPalette::ToolTipBase, Qt::black);
    pal.setBrush(QPalette::ToolTipText, Qt::white);
    pal.setBrush(QPalette::Text, QColor(0xd5, 0xc7, 0xa9));
    pal.setBrush(QPalette::Button, Qt::black);
    pal.setBrush(QPalette::ButtonText, QColor(0xd5, 0xc7, 0xa9));
    pal.setBrush(QPalette::BrightText, Qt::white);
    pal.setBrush(QPalette::Highlight, Qt::gray);
    pal.setBrush(QPalette::HighlightedText, Qt::yellow);
    app.setPalette(pal);

    MainWindow w;
    w.show();

    return app.exec();
}
