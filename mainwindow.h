#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prikaz.h"
#include "ansamblsignala.h"
#include "procesor.h"
#include "manipulacijaprocesorima.h"
#include "signalsexport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    //void onTableWidgetItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void onItemSelectionChanged();


    void on_pushButton_importFile_clicked();
    void on_pushButton_exportFile_clicked();
    void on_pushButton_Refresh_clicked();

private:

    void populateTableWidget_zaSignale(AnsamblSignala*& ansamblSignala);

    //Izbaciti
    void citajIzMatFajla(const QString& filePath, AnsamblSignala*& ansamblSignala);

    Ui::MainWindow *ui;
    QListWidget *listWidget;
    Prikaz prikaz1;
    Prikaz prikaz2;    

    //ManipulacijaProcesorima manipulatorProc;
    //Procesor defaultniProcesor;

    AnsamblSignala* pAnsamblSignala;
    ManipulacijaProcesorima* pManProc;
    Signal* signalUnderAnalysis;
    SignalsExport* pSigExp;

public:
    void setPointerAnsamblSignala(AnsamblSignala* _ptr){pAnsamblSignala = _ptr;}
    void setPointerManipulacijaProcesora(ManipulacijaProcesorima* _ptr){pManProc = _ptr;}

    //SignalsExport sigExpObj;

};
#endif // MAINWINDOW_H
