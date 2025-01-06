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

    if (tip_prikaza == "izl")
    {
        //Dodavanje koncanice

        // Kreirajte graf
        QCPGraph* graf_koncanica = qplot->addGraph();

        // Dodajte podatke za vertikalnu liniju od [vrij, -200] do [vrij, +200]
        QVector<double> x(2), y(2);
        x[0] = get_koncanica()*0.001;  // x koordinata početne točke
        x[1] = get_koncanica()*0.001;  // x koordinata krajnje točke
        y[0] = -10000;  // y koordinata početne točke
        y[1] = 10000;   // y koordinata krajnje točke

        // Postavite podatke za graf
        graf_koncanica->setData(x, y);

        // Postavite liniju kao crtanu (dashed) i crvene boje
        graf_koncanica->setPen(QPen(Qt::red, 2, Qt::DashLine));
        graf_koncanica->setName("Cursor");

        //Zbog milisekundi se mnozi sa 0.001
        qplot->xAxis->setRange(get_initTime()*0.001,get_initTime()*0.001+get_durationTime()*0.001);
    }
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
            //Stari pristup
            //QVector<double> x = pSignal->get_xData_izl_resampled();
            //QVector<double> y = pSignal->get_yData_izl_resampled();
            //Cutted vektori
            QVector<double> x;
            QVector<double> y;
            //Novi pristup:
            pSignal->get_xData_yData_from_to(x, y, 0.001*get_initTime(), 0.001*(get_initTime()+get_durationTime()));


            int signal_pos = pSignal->get_signal_position();
            double signal_size = pSignal->get_signal_size();

            // Izračunavanje srednje vrijednosti signala
            double mean = std::accumulate(y.begin(), y.end(), 0.0) / y.size();

            // Uklanjanje istosmjerne komponente i skaliranje
            QVector<double> scaled_y(y.size());
            double scale_factor = signal_size / 10.0; // Skaliranje prema signal_size
            for (int i = 0; i < y.size(); ++i) {
                scaled_y[i] = signal_pos + ((y[i] - mean) * scale_factor); // Uklanjanje DC, skaliranje, i shiftanje
            }


            graph->setData(x, scaled_y, true);

            //graph->setData(x,y, true);
        }

        graph->setLineStyle(QCPGraph::lsLine);
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
        graph->setPen(boja);

}


void Prikaz::inicijalizirajKoncanicu()
{

    // Podesite slider i spin box granice
    horizontalSlider_koncanica->setRange(0, 100);  // Slider radi s cijelim brojevima
    doubleSpinBox_koncanica->setRange(get_min_koncanica(), get_max_koncanica());
    doubleSpinBox_koncanica->setDecimals(2);       // Preciznost za decimalne brojeve

    // Povezivanje slidera i spinBox-a s varijablom
    connect(horizontalSlider_koncanica, &QSlider::valueChanged, this, [&]() {
        double newValue = get_min_koncanica() + (get_max_koncanica() - get_min_koncanica()) *
                          horizontalSlider_koncanica->value() / 100.0;
        set_koncanica(newValue);
        doubleSpinBox_koncanica->blockSignals(true);
        doubleSpinBox_koncanica->setValue(get_koncanica());
        doubleSpinBox_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

    connect(doubleSpinBox_koncanica, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double newValue) {
        set_koncanica(newValue);
        int sliderValue = static_cast<int>((get_koncanica() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0);
        horizontalSlider_koncanica->blockSignals(true);
        horizontalSlider_koncanica->setValue(sliderValue);
        horizontalSlider_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

}

// Ažuriranje granica pri promjeni min_koncanica ili max_koncanica
void Prikaz::azurirajGraniceKoncanice() {
    doubleSpinBox_koncanica->setRange(get_min_koncanica(), get_max_koncanica());
    double sliderValue = (get_koncanica() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0;
    horizontalSlider_koncanica->setValue(static_cast<int>(sliderValue));
};
