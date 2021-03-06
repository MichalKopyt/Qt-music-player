/*
 * File name: main.cpp
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Qt MP3 Player");
    w.setWindowIcon(QIcon(":/images/albumIcon.png"));
    w.show();
    w.setDefaultCover();
    return a.exec();
}
