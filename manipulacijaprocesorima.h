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
    Procesor* getPointerNaOdabraniProcesor(){return pOdabraniProcesor;}

    Procesor* getPointerNaDefaultniProcesor(){return pDefaultniProcesor;}

    void dodajProcesor(Procesor* procesor){vektorProcesora.push_back(procesor); populateTableWidget();}


    int i=0;
private:
    Ui::ManipulacijaProcesorima *ui;
    Procesor* pOdabraniProcesor = nullptr;
    Procesor* pDefaultniProcesor = nullptr;

    std::vector<Procesor*> vektorProcesora;


    void populateTableWidget();

    double MarkerValue = 0.0; //To je pocetna vrijednost signala u odnosu na referentnu tacku

public slots:

    void onChangedMarkerValue(double value){MarkerValue = value;}

private slots:
    void onProcessorOsvjezi();


private slots:

    void onItemSelectionChanged();
    void onTableItemDoubleClicked(int row, int column);
    void on_pushButton_newProcessor_clicked();
    void on_pushButton_editProcessor_clicked();
};

#endif // MANIPULACIJAPROCESORIMA_H
