#include "signal.h"
#include <QVector>

Signal::Signal(QObject *parent) : QObject(parent)
{

}

void Signal::ucitajSignalIzMatlabVarijable(matvar_t* matvar)
{
    imeSignala = matvar->name;
    tipPodatka = matvar->class_type;


    //Ucitavanje matrice tipa double-----------------------------------:
    if (matvar->rank == 2 && tipPodatka == MAT_C_DOUBLE)
    {
        double *data = (double *)matvar->data;
        std::vector<std::vector<double>> matrica_C_Double;

        for (size_t i = 0; i < matvar->dims[1]; ++i) {
            std::vector<double> red;
            for (size_t j = 0; j < matvar->dims[0]; ++j) {
                double vrijednost = data[i * matvar->dims[0] + j];
                //double vrijednost = data[j * matvar->dims[1] + i];
                red.push_back(vrijednost);
            }
            matrica_C_Double.push_back(red);
        }


        //PREPRAVITI
        xData.clear(); yData.clear();
        for (ulong i =0; i<matvar->dims[0]; i++)
        {
            xData.push_back(matrica_C_Double[0][i]);
            if (QString::fromStdString(matvar->name) != "MarkerValue")
            {
                yData.push_back(matrica_C_Double[1][i]);
            }
        }


    }
}


void Signal::podesiQCPgraph(QCPGraph* vanjski_graph)
{
    graph = vanjski_graph;

    graph->setName(ime());
    graph->data()->clear();

    //Crtanje originalnih podataka
    graph->setData(get_xData(), get_yData());

    //Prikaz originalnih podataka
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
    //graph_org->setPen(QPen(Qt::blue));

}

void Signal::dodajSignalULegendu(QCPLegend *legendica)
{
    graph->addToLegend(legendica);
}

/*
QVector<double> Signal::get_xData()
{
    QVector<double> xData = {0,0};
    if (getDimenzionalnostPodatka() == 2 && tipPodatka == MAT_C_DOUBLE)
    {
        xData.clear();
        //Iscitava nultu kolonu
        for (ulong i =0; i<getDimenzijaPodatka(0); i++)
        {
            xData.push_back(Matrica_C_Double[0][i]);
        }

    }
    return xData;
}
*/
/*
QVector<double> Signal::get_yData()
{
    QVector<double> yData = {0,0};
    if (getDimenzionalnostPodatka() == 2 && tipPodatka == MAT_C_DOUBLE)
    {
        yData.clear();
        //Iscitava nultu kolonu
        for (ulong i =0; i<getDimenzijaPodatka(0); i++)
        {
            yData.push_back(Matrica_C_Double[1][i]);
        }

    }
    return yData;
}

*/
void Signal::ucitajSignalIzDrugogSignala(Signal* signal)
{
    imeSignala = signal->ime();
    tipPodatka = signal->tipPodatka;

    set_xData(signal->get_xData());
    set_yData(signal->get_yData());
}




