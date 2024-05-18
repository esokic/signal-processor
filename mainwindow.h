#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prikaz.h"
#include "ansamblsignala.h"
#include "procesor.h"
#include "manipulacijaprocesorima.h"

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
    void on_pushButton_Refresh_clicked();

    void onListWidgetItemChanged(QListWidgetItem *current, QListWidgetItem *previous);


    void on_pushButton_importFile_clicked();

    void on_pushButton_exportFile_clicked();

private:
    void citajIzMatFajla(const QString& filePath, AnsamblSignala& ansamblSignala);


    Ui::MainWindow *ui;
    Prikaz prikaz1;
    Prikaz prikaz2;

    Signal* signalUnderAnalysis;
    //Signal* testout;

    //Procesor* pTrenutni_procesor_signala;
    ManipulacijaProcesorima manipulatorProc;

    QListWidget *listWidget;

};
#endif // MAINWINDOW_H
