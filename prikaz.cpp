#include "prikaz.h"
#include "qcustomplot.h"

Prikaz::Prikaz(QObject *parent) : QObject(parent)
{

}

void Prikaz::osvjeziPrikaz()
{
    //Ciscenje svega
    if (qplot->graphCount()>0) qplot->clearGraphs();
    if (qplot->itemCount()>0) qplot->clearItems();
    if (qplot->plottableCount()>0) qplot->clearPlottables();


    //Crtanje legende
    QCPLegend *legendica = new QCPLegend();
    legendica->setBrush(QBrush(Qt::white));
    legendica->setBorderPen(QPen(Qt::black));
    qplot->axisRect()->insetLayout()->addElement(legendica, Qt::AlignBottom | Qt::AlignLeft);
    qplot->legend = legendica;




    for (Signal* pSignal : vektor_pSignala)
    {
        pSignal->podesiQCPgraph(qplot->addGraph());


      //  qplot->xAxis->setLabel(pVekMjerenja->legendaXosa);
      //  qplot->yAxis->setLabel(pVekMjerenja->legendaYosa);
      //  qplot->xAxis->setScaleType(skalaXose);
      //  qplot->xAxis->setTickLabelRotation(45);
    }

    // Prikazivanje dijagrama
   /* if (b_set_prikaz_ose_manuelno == true)
    {

        qplot->replot();
    } else
    {
        qplot->rescaleAxes();
        qplot->replot();

    }*/
    qplot->rescaleAxes();
    qplot->replot();


}
