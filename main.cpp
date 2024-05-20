#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <matio.h>
#include <signal.h>

#include <ansamblsignala.h>

#include "signalsexport.h"





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
