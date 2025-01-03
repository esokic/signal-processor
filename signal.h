#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>
#include <matio.h>
#include <qcustomplot.h>
#include "procesor.h"

class Signal : public QObject
{
    Q_OBJECT
public:
    explicit Signal(QObject *parent = nullptr);



    void ucitajSignalIzMatlabVarijable(matvar_t*);
    void ucitajSignalIzDrugogSignala(Signal*& signal); //prakticno kopija

    //Kljucna funkcija
    void procesirajSignal(){if (pProcesor!=nullptr) {pProcesor->procesiraj(xData_ul,yData_ul,xData_izl,yData_izl);}}

   // void promijeni_startTime(double t);


    //getteri
    QString ime(){return imeSignala;}

    /*
    QString ispisi_dimenzije(){return QString::number(dimenzijePodatka[0])+"x"+
                QString::number(dimenzijePodatka[1]);}
*/

    QVector<double> get_xData_ul(){return xData_ul;}
    QVector<double> get_yData_ul(){return yData_ul;}
    QVector<double> get_xData_izl(){return xData_izl;}
    QVector<double> get_yData_izl(){return yData_izl;}

    double getMarkerValue(){return MarkerValue;}
    bool isMarkerValueAssigned(){return markerValueAssigned;}

    QString getNewName(){return novoImeSignala;}
    bool isMarkedForExport(){return oznacen_za_export;}
    bool isMarkedForPrikaz(){return oznacen_za_prikaz;}

    //setteri
    //Sluzi za podesavanje prikaza ovog signala
    void podesiQCPgraph(QCPGraph*, QString tip_grafika); //moze biti "ul" i "izl"
    void dodajSignalULegendu(QCPLegend*);

    //Ovdje ima problem jer se mora paziti da se sve mijenja
    void set_xData_ul(QVector<double> xd){xData_ul = xd;}
    void set_yData_ul(QVector<double> yd){yData_ul = yd;}
    void set_xData_izl(QVector<double> xd){xData_ul = xd;}
    void set_yData_izl(QVector<double> yd){yData_ul = yd;}
    void setMarkerValue(double _mv){MarkerValue = _mv; markerValueAssigned = true;}
    void set_novoIme(QString _ime){novoImeSignala = _ime;}
    void setMarkedForExport(bool _smfe){oznacen_za_export = _smfe;}
    void setMarkedForPrikaz(bool _smfe){oznacen_za_prikaz = _smfe;}

    void setPointerNaProcesor(Procesor* _pProcesor){pProcesor = _pProcesor;}
    //Procesor* getPointerNaProcesor(){return pProcesor;}

    QString getTrenutniProcesorIme(){if(pProcesor!=nullptr){return pProcesor->getIme();}else{return "---------";}}
    QDateTime getTrenutniProcesorUpdateTime(){if(pProcesor!=nullptr){return pProcesor->getLastUpdateTime();}else{return QDateTime::currentDateTime();}}
private:
    QString imeSignala;                 //Npr. U1
    QString tipPodatka;                 //Npr. MAT_C_DOUBLE

    bool oznacen_za_export = false;
    bool oznacen_za_prikaz= false;
    QString novoImeSignala;

    QVector<double> xData_ul;
    QVector<double> yData_ul;

    QVector<double> xData_izl;
    QVector<double> yData_izl;
    //Ogranicit cemo se na dvije dimenzije i jednake dimenzije signala

    double MarkerValue = 0.0; //Ovo je za onaj specificni fajl od KEME
    bool markerValueAssigned = false;

    //Pomocni pokazivac na graph
    QCPGraph *graph;


    Procesor* pProcesor = nullptr;

signals:

};

#endif // SIGNAL_H
