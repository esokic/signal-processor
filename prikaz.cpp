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
        //Dodavanje koncanice - 1 ---------------------------------------------------
        QCPGraph* graf_koncanica_1 = qplot->addGraph();
        // Dodajte podatke za vertikalnu liniju od [vrij, -200] do [vrij, +200]
        QVector<double> x(2), y(2);
        //x[0] = get_koncanica()*0.001;  // x koordinata početne točke
        //x[1] = get_koncanica()*0.001;  // x koordinata krajnje točke
        x[0] = get_koncanica_1();  // x koordinata početne točke
        x[1] = get_koncanica_1();  // x koordinata krajnje točke
        y[0] = -10000;  // y koordinata početne točke
        y[1] = 10000;   // y koordinata krajnje točke
        // Postavite podatke za graf
        graf_koncanica_1->setData(x, y);
        // Postavite liniju kao crtanu (dashed) i crvene boje
        graf_koncanica_1->setPen(QPen(Qt::red, 2, Qt::DashLine));
        graf_koncanica_1->setName("Cursor 1");
        //Zbog milisekundi se mnozi sa 0.001
        //qplot->xAxis->setRange(get_initTime()*0.001,get_initTime()*0.001+get_durationTime()*0.001);

        //Dodavanje koncanice - 1 ---------------------------------------------------
        QCPGraph* graf_koncanica_2= qplot->addGraph();
        // Dodajte podatke za vertikalnu liniju od [vrij, -200] do [vrij, +200]
        QVector<double> xx(2), yy(2);
        //x[0] = get_koncanica()*0.001;  // x koordinata početne točke
        //x[1] = get_koncanica()*0.001;  // x koordinata krajnje točke
        xx[0] = get_koncanica_2();  // x koordinata početne točke
        xx[1] = get_koncanica_2();  // x koordinata krajnje točke
        yy[0] = -10000;  // y koordinata početne točke
        yy[1] = 10000;   // y koordinata krajnje točke
        // Postavite podatke za graf
        graf_koncanica_2->setData(xx, yy);
        // Postavite liniju kao crtanu (dashed) i crvene boje
        graf_koncanica_2->setPen(QPen(Qt::red, 2, Qt::DashLine));
        graf_koncanica_2->setName("Cursor 2");
        //Zbog milisekundi se mnozi sa 0.001
        //qplot->xAxis->setRange(get_initTime()*0.001,get_initTime()*0.001+get_durationTime()*0.001);





        qplot->xAxis->setRange(get_initTime(),get_initTime()+get_durationTime());
        qplot->yAxis->setRange(0,100);
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
            //pSignal->get_xData_yData_from_to(x, y, 0.001*get_initTime(), 0.001*(get_initTime()+get_durationTime()));
            pSignal->get_xData_yData_from_to(x, y, get_initTime(), (get_initTime()+get_durationTime()));



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
        set_koncanica_1(newValue);
        doubleSpinBox_koncanica->blockSignals(true);
        doubleSpinBox_koncanica->setValue(get_koncanica_1());
        doubleSpinBox_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

    connect(doubleSpinBox_koncanica, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double newValue) {
        set_koncanica_1(newValue);
        int sliderValue = static_cast<int>((get_koncanica_1() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0);
        horizontalSlider_koncanica->blockSignals(true);
        horizontalSlider_koncanica->setValue(sliderValue);
        horizontalSlider_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

}

void Prikaz::inicijalizirajKoncanicu_2()
{

    // Podesite slider i spin box granice
    horizontalSlider_koncanica_2->setRange(0, 100);  // Slider radi s cijelim brojevima
    doubleSpinBox_koncanica_2->setRange(get_min_koncanica(), get_max_koncanica());
    doubleSpinBox_koncanica_2->setDecimals(2);       // Preciznost za decimalne brojeve

    // Povezivanje slidera i spinBox-a s varijablom
    connect(horizontalSlider_koncanica_2, &QSlider::valueChanged, this, [&]() {
        double newValue = get_min_koncanica() + (get_max_koncanica() - get_min_koncanica()) *
                          horizontalSlider_koncanica_2->value() / 100.0;
        set_koncanica_2(newValue);
        doubleSpinBox_koncanica_2->blockSignals(true);
        doubleSpinBox_koncanica_2->setValue(get_koncanica_2());
        doubleSpinBox_koncanica_2->blockSignals(false);
        koncanicaOsvjezena();
    });

    connect(doubleSpinBox_koncanica_2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double newValue) {
        set_koncanica_2(newValue);
        int sliderValue = static_cast<int>((get_koncanica_2() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0);
        horizontalSlider_koncanica_2->blockSignals(true);
        horizontalSlider_koncanica_2->setValue(sliderValue);
        horizontalSlider_koncanica_2->blockSignals(false);
        koncanicaOsvjezena();
    });

}

// Ažuriranje granica pri promjeni min_koncanica ili max_koncanica
void Prikaz::azurirajGraniceKoncanice() {
    doubleSpinBox_koncanica->setRange(get_min_koncanica(), get_max_koncanica());
    double sliderValue = (get_koncanica_1() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0;
    horizontalSlider_koncanica->setValue(static_cast<int>(sliderValue));

    doubleSpinBox_koncanica_2->setRange(get_min_koncanica(), get_max_koncanica());
    double sliderValue_2 = (get_koncanica_2() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * 100.0;
    horizontalSlider_koncanica_2->setValue(static_cast<int>(sliderValue_2));
};



void Prikaz::osvjeziTabeluKoncanica()
{
    tabelaTrenutnaKoncanica->clearContents();
    tabelaTrenutnaKoncanica->setRowCount(4);
    tabelaTrenutnaKoncanica->setColumnCount(vektor_pSignala.size()+2);


    QTableWidgetItem *dateItem1 = new QTableWidgetItem("Cursor 1");
    dateItem1->setFlags(dateItem1->flags() & ~Qt::ItemIsEditable);
    tabelaTrenutnaKoncanica->setItem(1, 0, dateItem1);


    QTableWidgetItem *dateItem2 = new QTableWidgetItem("Cursor 2");
    dateItem2->setFlags(dateItem2->flags() & ~Qt::ItemIsEditable);
    tabelaTrenutnaKoncanica->setItem(2, 0, dateItem2);

    QTableWidgetItem *dateItem3 = new QTableWidgetItem("Diff");
    dateItem3->setFlags(dateItem3->flags() & ~Qt::ItemIsEditable);
    tabelaTrenutnaKoncanica->setItem(3, 0, dateItem3);



    QTableWidgetItem* item0 = tabelaTrenutnaKoncanica->item(0, 1);
    if (!item0) {
        item0 = new QTableWidgetItem();
        tabelaTrenutnaKoncanica->setItem(0, 1, item0);
    }
    item0->setText("Time [ms]");

    QTableWidgetItem* item = tabelaTrenutnaKoncanica->item(1, 1);
    if (!item) {
        item = new QTableWidgetItem();
        tabelaTrenutnaKoncanica->setItem(1, 1, item);
    }
    item->setText(QString::number(get_koncanica_1()));

    QTableWidgetItem* item2 = tabelaTrenutnaKoncanica->item(2, 1);
    if (!item2) {
        item2 = new QTableWidgetItem();
        tabelaTrenutnaKoncanica->setItem(2, 1, item2);
    }
    item2->setText(QString::number(get_koncanica_2()));

    QTableWidgetItem* item3 = tabelaTrenutnaKoncanica->item(3, 1);
    if (!item3) {
        item3 = new QTableWidgetItem();
        tabelaTrenutnaKoncanica->setItem(3, 1, item3);
    }
    item3->setText(QString::number(get_koncanica_1()-get_koncanica_2()));



    int row = 2;
    for (auto signal: vektor_pSignala)
    {
        //Sad popuni nazive signala
        QTableWidgetItem* item = tabelaTrenutnaKoncanica->item(0, row);
        if (!item) {
            item = new QTableWidgetItem();
            tabelaTrenutnaKoncanica->setItem(0, row, item);
        }
        item->setText(signal->ime());

        QTableWidgetItem* item1 = tabelaTrenutnaKoncanica->item(1, row);
        if (!item1) {
            item1 = new QTableWidgetItem();
            tabelaTrenutnaKoncanica->setItem(1, row, item1);
        }
        double vrij_signala_t1 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_1());
        item1->setText(QString::number(vrij_signala_t1));

        QTableWidgetItem* item2 = tabelaTrenutnaKoncanica->item(2, row);
        if (!item2) {
            item2 = new QTableWidgetItem();
            tabelaTrenutnaKoncanica->setItem(2, row, item2);
        }
        double vrij_signala_t2 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_2());
        item2->setText(QString::number(vrij_signala_t2));

        QTableWidgetItem* item3 = tabelaTrenutnaKoncanica->item(3, row);
        if (!item3) {
            item3 = new QTableWidgetItem();
            tabelaTrenutnaKoncanica->setItem(3, row, item3);
        }
        item3->setText(QString::number(vrij_signala_t1-vrij_signala_t2));




        row++;
    }

    tabelaTrenutnaKoncanica->resizeColumnsToContents();
}
