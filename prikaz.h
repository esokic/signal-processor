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

    QString getTipPrikaza(){return tip_prikaza;}
    void setTipPrikaza(QString _tp){tip_prikaza = _tp;}


signals:



private:
       std::vector<Signal*> vektor_pSignala;

       QCustomPlot* qplot;
       QCPLegend *legend;

       QString tip_prikaza = "ul";

       //QCPAxis::ScaleType skalaXose = QCPAxis::stLogarithmic;

       double xmin = 0.0;
       double xmax = 1000.0;
       double ymin = 0.0;
       double ymax = 40000.0;



       // Lista boja koje će se koristiti za signale
       QVector<QColor> boje = {
           Qt::blue, Qt::red, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta, Qt::gray,
           QColor(255, 165, 0), QColor(255, 105, 180), QColor(0, 255, 127), QColor(255, 20, 147),
           QColor(240, 128, 128), QColor(138, 43, 226), QColor(0, 255, 255), QColor(255, 0, 255),
           QColor(0, 0, 255), QColor(255, 0, 0), QColor(0, 128, 0), QColor(255, 255, 0),
           QColor(128, 0, 128), QColor(128, 128, 0), QColor(0, 128, 128), QColor(255, 215, 0),
           QColor(105, 105, 105), QColor(255, 69, 0), QColor(139, 0, 0), QColor(75, 0, 130),
           QColor(255, 215, 180), QColor(0, 191, 255), QColor(32, 178, 170), QColor(255, 99, 71),
           QColor(0, 250, 154), QColor(244, 164, 96), QColor(0, 128, 255), QColor(100, 149, 237),
           QColor(205, 92, 92), QColor(255, 140, 0), QColor(255, 182, 193), QColor(46, 139, 87),
           QColor(70, 130, 180), QColor(255, 228, 196), QColor(240, 230, 140), QColor(139, 69, 19),
           QColor(0, 255, 255), QColor(255, 182, 193), QColor(50, 205, 50), QColor(160, 82, 45)
       };


};

#endif // PRIKAZ_H
