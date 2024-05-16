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
    QString ime(){return imeSignala;}
    QString ispisi_dimenzije(){return QString::number(dimenzijePodatka[0])+"x"+
                QString::number(dimenzijePodatka[1]);}

    //Sluzi za podesavanje prikaza ovog signala
    void podesiQCPgraph(QCPGraph*);
    void dodajSignalULegendu(QCPLegend*);

    QVector<double> get_xData();
    QVector<double> get_yData();

private:
    QString imeSignala;                 //Npr. U1
    QString tipPodatka;                 //Npr. MAT_C_DOUBLE
    std::vector<ulong> dimenzijePodatka; //Npr. 20,30 oznacava matricu 20x30

    std::vector<std::vector<double>> Matrica_C_Double;
    //QVector<double> xData = {0,0};
    //QVector<double> yData = {0,0};

    ulong getDimenzionalnostPodatka(){return dimenzijePodatka.size();}
    ulong getDimenzijaPodatka(ulong i){
        if (dimenzijePodatka.size()>0) {return dimenzijePodatka[i];} else {return 0;}}

    QCPGraph *graph;
signals:

};

#endif // SIGNAL_H
