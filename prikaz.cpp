#include "prikaz.h"
#include "qcustomplot.h"

Prikaz::Prikaz(QObject *parent) : QObject(parent)
{

}

void Prikaz::osvjeziPrikaz()
{

    // Uklanjanje stare legende
    if (qplot->legend) {
        qplot->axisRect()->insetLayout()->remove(qplot->legend);
        delete qplot->legend;
        qplot->legend = nullptr;
    }


    // Brisanje svih grafova
    if (qplot->graphCount()>0) qplot->clearGraphs();
    if (qplot->itemCount()>0) qplot->clearItems();
    if (qplot->plottableCount()>0) qplot->clearPlottables();

    // Kreiranje nove legende
    QCPLegend *legendica = new QCPLegend();
    legendica->setBrush(QBrush(Qt::white));
    legendica->setBorderPen(QPen(Qt::black));
    qplot->axisRect()->insetLayout()->addElement(legendica, Qt::AlignBottom | Qt::AlignLeft);
    qplot->legend = legendica;

    // Dodavanje grafova
    int index = 0;
    for (Signal* pSignal : vektor_pSignala) {
        QColor boja = boje[index % boje.size()];
        index++;
        QCPGraph* grafik = qplot->addGraph();
        this->podesiQCPgraphZaSignal(grafik, pSignal, getTipPrikaza(), boja);
    }

    // Ažuriranje prikaza
    qplot->rescaleAxes();
    qplot->replot();

    std::cout << "trenutni graph count" << qplot->graphCount() << std::endl;
}



void Prikaz::podesiQCPgraphZaSignal(QCPGraph*& graph, Signal* pSignal, QString tip_grafika, QColor boja)
{
        graph->setName(pSignal->ime());
        //Ovo je isto viska
        graph->data()->clear();



        if (tip_grafika == "ul") {
            //Moze se dodati alreadysorted=true da se jos ubrza
            graph->setAdaptiveSampling(true);
            //Prikaz je resampliran
            graph->setData(pSignal->get_xData_ul_resampled(), pSignal->get_yData_ul_resampled(), true);

        } else if (tip_grafika == "izl") {
            graph->setAdaptiveSampling(true);
            graph->setData(pSignal->get_xData_izl_resampled(), pSignal->get_yData_izl_resampled(), true);
        }

        graph->setLineStyle(QCPGraph::lsLine);
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
        graph->setPen(boja);

}
