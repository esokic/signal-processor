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

    ManipulacijaProcesorima manProc;

    AnsamblSignala ansamblSignala;
    ansamblSignala.setPointerNaManipulacijuProcesorima(&manProc);    

    SignalsExport sigExp;
    sigExp.setPtrAnsamblSignala(&ansamblSignala);


    MainWindow glavniProzor;
    glavniProzor.setPointerAnsamblSignala(&ansamblSignala);
    glavniProzor.setPointerManipulacijaProcesora(&manProc);
    glavniProzor.setPointerSignalExport(&sigExp);

    manProc.show();
    glavniProzor.show();




    return a.exec();
}
