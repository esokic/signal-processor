#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prikaz.h"
#include "ansamblsignala.h"
#include "procesor.h"

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
    void on_pushButton_AutoReadMarker_clicked();

    void onListWidgetItemChanged(QListWidgetItem *current, QListWidgetItem *previous);


private:
    void citajIzMatFajla(const QString& filePath, AnsamblSignala& ansamblSignala);


    Ui::MainWindow *ui;
    Prikaz prikaz1;
    Prikaz prikaz2;

    Signal* test;
    Signal* testout;

    Procesor procesor_signala;

    QListWidget *listWidget;

};
#endif // MAINWINDOW_H
