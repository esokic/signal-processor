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

    //void setPointerAnsamblSignala(AnsamblSignala* _pAnsamblSignala){pAnsamblSignala = _pAnsamblSignala;}

private slots:


    //void onTableWidgetItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void onItemSelectionChanged();


    void on_pushButton_importFile_clicked();
    void on_pushButton_exportFile_clicked();
    void on_pushButton_Refresh_clicked();

    QStringList dohvatiMatFajloveIzFoldera(const QString& folderPath);
    void ucitajNaredniFajl();
    void ucitajPrethodniFajl();

    void on_pushButton_prevFile_clicked();

    void on_pushButton_nextFile_clicked();

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


    //Sluzi za navigaciju desno lijevo kod izbora fajla unutar foldera
    QStringList matFajlovi;
    int trenutniIndeks;
    QString folderPath;

public:
    void setPointerAnsamblSignala(AnsamblSignala* _ptr){pAnsamblSignala = _ptr;}
    void setPointerManipulacijaProcesora(ManipulacijaProcesorima* _ptr){pManProc = _ptr;}
    void setPointerSignalExport(SignalsExport* _pSigExp){pSigExp = _pSigExp;}

    //SignalsExport sigExpObj;

};
#endif // MAINWINDOW_H
