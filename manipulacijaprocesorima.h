#ifndef MANIPULACIJAPROCESORIMA_H
#define MANIPULACIJAPROCESORIMA_H

#include <QWidget>
#include "procesor.h"

namespace Ui {
class ManipulacijaProcesorima;
}

class ManipulacijaProcesorima : public QWidget
{
    Q_OBJECT

public:
    explicit ManipulacijaProcesorima(QWidget *parent = nullptr);
    ~ManipulacijaProcesorima();

    void setPointerNaOdabraniProcesor(Procesor* _pOdabraniProcesor){pOdabraniProcesor = _pOdabraniProcesor;}

    void dodajProcesor(Procesor* procesor){vektorProcesora.push_back(procesor); populateTableWidget();}

    int i=0;
private:
    Ui::ManipulacijaProcesorima *ui;
    Procesor* pOdabraniProcesor;

    std::vector<Procesor*> vektorProcesora;


    void populateTableWidget();

public slots:
    void onItemSelectionChanged();

private slots:

    void on_pushButton_newProcessor_clicked();
};

#endif // MANIPULACIJAPROCESORIMA_H
