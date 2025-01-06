#include "signal.h"
#include <QVector>


Signal::Signal(QObject *parent) : QObject(parent)
{

}

void Signal::ucitajSignalIzMatlabVarijable(matvar_t* matvar)
{
    imeSignala = matvar->name;
    novoImeSignala = imeSignala;
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
        xData_ul.clear(); yData_ul.clear();
        for (ulong i =0; i<matvar->dims[0]; i++)
        {
            xData_ul.push_back(matrica_C_Double[0][i]);
            if (QString::fromStdString(matvar->name) != "MarkerValue")
            {
                yData_ul.push_back(matrica_C_Double[1][i]);
            } else
            {
                MarkerValue = matrica_C_Double[0][i];
                markerValueAssigned = true;
                //Prosiriti ovo jos da se moze nacrtati
                xData_ul.push_back(xData_ul[0]);
                yData_ul.clear();
                yData_ul.push_back(-10000.0);
                yData_ul.push_back(10000.0);
            }
        }

        //Ako se mjeri u milisekundama - ovo vidjeti da li da se transformira ili ne?
        for (int i=0; i<xData_ul.size(); i++)
        {
            xData_ul[i] = xData_ul[i]*1000.0;
        }
        xData_izl = xData_ul;
        yData_izl = yData_ul;



    }
}

QVector<double> Signal::resampleData(const QVector<double>& data, int N) {
    int originalSize = data.size();
    if (originalSize <= N) {
        return data;  // Ako je broj uzoraka manji ili jednak N, ne radimo resempliranje
    }

    QVector<double> resampledData;
    double step = static_cast<double>(originalSize) / N;

    for (int i = 0; i < N; ++i) {
        int index = static_cast<int>(i * step);
        resampledData.append(data[index]);
    }

    return resampledData;
}

/*
void Signal::dodajSignalULegendu(QCPLegend *legendica)
{
    graph->addToLegend(legendica);
}
*/

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
void Signal::ucitajSignalIzDrugogSignala(Signal*& signal)
{
    imeSignala = signal->imeSignala;
    novoImeSignala = signal->novoImeSignala;
    tipPodatka = signal->tipPodatka;

    set_xData_ul(signal->get_xData_ul());
    set_yData_ul(signal->get_yData_ul());
}

void Signal::kopirajPodesenjaIzDrugogSignala(const Signal& drugiSignal)
{
    this->oznacen_za_export = drugiSignal.oznacen_za_export;
    this->oznacen_za_prikaz = drugiSignal.oznacen_za_prikaz;
    this->novoImeSignala = drugiSignal.novoImeSignala;
    this->pProcesor = drugiSignal.pProcesor;  // Pokazivač na isti procesor, ne kopiramo objekat
}

void Signal::get_xData_yData_from_to(QVector<double>& xData_cutted, QVector<double>& yData_cutted, double start_time, double end_time)
{
// Iteriranje kroz podatke
    for (int i = 0; i < xData_izl.size(); ++i) {
        double time = xData_izl.at(i);
        if (time >= start_time && time <= end_time) {
            // Dodavanje odgovarajućih podataka u ciljne vektore
            xData_cutted.append(time);
            yData_cutted.append(yData_izl.at(i));
        }
    }
}

double Signal::vratiVrijednostSignalaUtrenutku(double time_x) {

    // Pronađi indeks najbliže vrijednosti u xData_izl
    auto it = std::lower_bound(xData_izl.begin(), xData_izl.end(), time_x);
    int idx = std::distance(xData_izl.begin(), it);

    // Ako je tačna vrijednost pronađena, vrati odgovarajući y
    if (idx < xData_izl.size() && xData_izl[idx] == time_x) {
        return yData_izl[idx];
    }

    // Interpolacija između susjednih tačaka
    int idx1 = std::max(0, idx - 1);       // Donji indeks
    int idx2 = std::min(idx, xData_izl.size() - 1); // Gornji indeks

    if (idx1 == idx2) {
        return yData_izl[idx1]; // Samo jedna tačka, vrati njenu vrijednost
    }

    double x1 = xData_izl[idx1];
    double x2 = xData_izl[idx2];
    double y1 = yData_izl[idx1];
    double y2 = yData_izl[idx2];

    // Linearna interpolacija
    return y1 + (y2 - y1) * (time_x - x1) / (x2 - x1);
}




