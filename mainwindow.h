#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prikaz.h"
#include "ansamblsignala.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void citajIzMatFajla(const QString& filePath, AnsamblSignala& ansamblSignala);


    Ui::MainWindow *ui;
    Prikaz prikaz1;
    Prikaz prikaz2;
};
#endif // MAINWINDOW_H
