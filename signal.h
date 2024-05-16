#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>
#include <matio.h>
#include <qcustomplot.h>

class Signal : public QObject
{
    Q_OBJECT
public:
    explicit Signal(QObject *parent = nullptr);


    void ucitajSignalIzMatlabVarijable(matvar_t*);
    void ucitajSignalIzDrugogSignala(Signal*& signal); //prakticno kopija


   // void promijeni_startTime(double t);


    //getteri
    QString ime(){return imeSignala;}

    /*
    QString ispisi_dimenzije(){return QString::number(dimenzijePodatka[0])+"x"+
                QString::number(dimenzijePodatka[1]);}
*/
    QVector<double> get_xData(){return xData;}
    QVector<double> get_yData(){return yData;}
    double getMarkerValue(){return MarkerValue;}
    bool isMarkerValueAssigned(){return markerValueAssigned;}



    //setteri
    //Sluzi za podesavanje prikaza ovog signala
    void podesiQCPgraph(QCPGraph*);
    void dodajSignalULegendu(QCPLegend*);

    //Ovdje ima problem jer se mora paziti da se sve mijenja
    void set_xData(QVector<double> xd){xData = xd;}
    void set_yData(QVector<double> yd){yData = yd;}
    void setMarkerValue(double _mv){MarkerValue = _mv; markerValueAssigned = true;}



private:
    QString imeSignala;                 //Npr. U1
    QString tipPodatka;                 //Npr. MAT_C_DOUBLE

    QVector<double> xData;
    QVector<double> yData;
    //Ogranicit cemo se na dvije dimenzije i jednake dimenzije signala

    double MarkerValue = 0.0; //Ovo je za onaj specificni fajl od KEME
    bool markerValueAssigned = false;

    //Pomocni pokazivac na graph
    QCPGraph *graph;
signals:

};

#endif // SIGNAL_H
