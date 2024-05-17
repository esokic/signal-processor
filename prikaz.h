#ifndef PRIKAZ_H
#define PRIKAZ_H

#include <QObject>
#include <qcustomplot.h>
#include <signal.h>


class Prikaz : public QObject
{
    Q_OBJECT
public:
    explicit Prikaz(QObject *parent = nullptr);

    void setPointerQPlot(QCustomPlot* _qplt){qplot = _qplt;}
    void dodajSignaluGrupuZaPrikaz(Signal* signal){vektor_pSignala.push_back(signal);}
    void ocistiPrikaz(){vektor_pSignala.clear();}

    void osvjeziPrikaz();


signals:



private:
       std::vector<Signal*> vektor_pSignala;

       QCustomPlot* qplot;
       QCPLegend *legend;

       //QCPAxis::ScaleType skalaXose = QCPAxis::stLogarithmic;

       double xmin = 0.0;
       double xmax = 1000.0;
       double ymin = 0.0;
       double ymax = 40000.0;




};

#endif // PRIKAZ_H
