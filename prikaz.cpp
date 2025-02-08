#include "prikaz.h"
#include "qcustomplot.h"
#include <xlsxdocument.h>
#include <QDialog>

Prikaz::Prikaz(QObject *parent) : QObject(parent)
{

}

void Prikaz::osvjeziPrikaz()
{

    // Uklanjanje stare legende
    /*
    if (qplot->legend) {
        qplot->axisRect()->insetLayout()->remove(qplot->legend);
        delete qplot->legend;
        qplot->legend = nullptr;
    }
    */


    // Brisanje svih grafova
    if (qplot->graphCount()>0) qplot->clearGraphs();
    //if (qplot->itemCount()>0) qplot->clearItems();
    //if (qplot->plottableCount()>0) qplot->clearPlottables();

    // Kreiranje nove legende
    /*
    QCPLegend *legendica = new QCPLegend();
    legendica->setBrush(QBrush(Qt::white));
    legendica->setBorderPen(QPen(Qt::black));
    qplot->axisRect()->insetLayout()->addElement(legendica, Qt::AlignBottom | Qt::AlignLeft);
    qplot->legend = legendica;
    */

    // Dodavanje grafova
    //int index = 0;
    //Svi grafici

    for (Signall* pSignal : vektor_pSignala) {
        //QColor boja = boje[index % boje.size()];
        //index++;
        QCPGraph* grafik = qplot->addGraph();
        this->podesiQCPgraphZaSignal(grafik, pSignal, getTipPrikaza());
    }
    /*
    QCPGraph* grafik = qplot->addGraph();
    this->podesiQCPgraphZaSignal(grafik, get_ptrAktivniSignal(), getTipPrikaza());
    //Samo selektovani grafik
*/
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
        //graf_koncanica_1->setPen(QPen(Qt::black, 2, Qt::DashLine));
        graf_koncanica_1->setPen(QPen(get_koncanica_1_color(), 1));
        graf_koncanica_1->setName("Cursor 1");
        //Zbog milisekundi se mnozi sa 0.001
        //qplot->xAxis->setRange(get_initTime()*0.001,get_initTime()*0.001+get_durationTime()*0.001);

       // labelica_koncanica1->setColor(get_koncanica_1_color());
       // labelica_koncanica1->position->setCoords(get_koncanica_1(), 3);            // Pozicija


        //Dodavanje koncanice - 2 ---------------------------------------------------
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
        //graf_koncanica_2->setPen(QPen(Qt::black, 2, Qt::DashDotDotLine));
        graf_koncanica_2->setPen(QPen(get_koncanica_2_color(), 1));
        graf_koncanica_2->setName("Cursor 2");
        //Zbog milisekundi se mnozi sa 0.001
        //qplot->xAxis->setRange(get_initTime()*0.001,get_initTime()*0.001+get_durationTime()*0.001);

       // labelica_koncanica2->position->setCoords(get_koncanica_2(), 3);            // Pozicija
       // labelica_koncanica2->setColor(get_koncanica_2_color());


        qplot->xAxis->setRange(get_initTime(),get_initTime()+get_durationTime());
        qplot->yAxis->setRange(0,100);
    }
    qplot->replot();

    //std::cout << "trenutni graph count" << qplot->graphCount() << std::endl;
    // Isključi interakciju za dragovanje osa

   //setupPlot_zaInteraktivniPravougaonik();

}



void Prikaz::podesiQCPgraphZaSignal(QCPGraph*& graph, Signall* pSignal, QString tip_grafika)
{
        graph->setName(pSignal->ime());
        //Ovo je isto viska
        graph->data()->clear();



        if (tip_grafika == "ul") {
            //Moze se dodati alreadysorted=true da se jos ubrza
           // graph->setAdaptiveSampling(true);
            //Prikaz je resampliran
            graph->setData(pSignal->get_xData_ul_resampled(), pSignal->get_yData_ul_resampled(), true);

        } else if (tip_grafika == "izl") {
           // graph->setAdaptiveSampling(true);
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
           // double mean = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
            double mean = 0; //Skalira se u odnosu na nultu vrijednost

            // Uklanjanje istosmjerne komponente i skaliranje
            QVector<double> scaled_y(y.size());
            double scale_factor = signal_size / 10.0; // Skaliranje prema signal_size
            for (int i = 0; i < y.size(); ++i) {
                scaled_y[i] = signal_pos + ((y[i] - mean) * scale_factor); // Uklanjanje DC, skaliranje, i shiftanje
            }

            //Original------------
            graph->setData(x, scaled_y, true);

            /*
            // Kreiraj kontejner podataka
            QSharedPointer<QCPGraphDataContainer> dataContainer(new QCPGraphDataContainer);

            // Dodaj podatke u kontejner
            for (int i = 0; i < x.size(); ++i)
            {
                QCPGraphData dataPoint;
                dataPoint.key = x[i];     // X koordinata
                dataPoint.value = scaled_y[i];   // Y koordinata
                dataContainer->add(dataPoint);
            }

            // Postavi podatke za graf
            graph->setData(dataContainer);
            */
            //graph->setData(x,y, true);
        }

        graph->setLineStyle(QCPGraph::lsLine);
        //graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
        graph->setPen(QPen(pSignal->getBojaSignala()));
}


void Prikaz::inicijalizirajKoncanicu()
{

    // Podesite slider i spin box granice
    horizontalSlider_koncanica->setRange(0, GRANULACIJA_KONCANICE);  // Slider radi s cijelim brojevima
    doubleSpinBox_koncanica->setRange(get_min_koncanica(), get_max_koncanica());
    doubleSpinBox_koncanica->setDecimals(2);       // Preciznost za decimalne brojeve

    // Povezivanje slidera i spinBox-a s varijablom
    connect(horizontalSlider_koncanica, &QSlider::valueChanged, this, [&]() {
        double newValue = get_min_koncanica() + (get_max_koncanica() - get_min_koncanica()) *
                          horizontalSlider_koncanica->value() / GRANULACIJA_KONCANICE;
        set_koncanica_1(newValue);
        doubleSpinBox_koncanica->blockSignals(true);
        doubleSpinBox_koncanica->setValue(get_koncanica_1());
        doubleSpinBox_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

    connect(doubleSpinBox_koncanica, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double newValue) {
        set_koncanica_1(newValue);
        int sliderValue = static_cast<int>((get_koncanica_1() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * GRANULACIJA_KONCANICE);
        horizontalSlider_koncanica->blockSignals(true);
        horizontalSlider_koncanica->setValue(sliderValue);
        horizontalSlider_koncanica->blockSignals(false);
        koncanicaOsvjezena();
    });

    // Dodavanje dinamičke oznake pored linije
    QCPItemText *label = new QCPItemText(qplot);
    // Postavi svojstva oznake
    label->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Poravnanje
    label->position->setType(QCPItemPosition::ptPlotCoords);       // Koordinate grafika
    label->setText("C1");                           // Postavi broj kao tekst
    label->setFont(QFont("Arial", 10));                           // Font i veličina

    labelica_koncanica1 = label;
}

void Prikaz::inicijalizirajKoncanicu_2()
{

    // Podesite slider i spin box granice
    horizontalSlider_koncanica_2->setRange(0, GRANULACIJA_KONCANICE);  // Slider radi s cijelim brojevima
    doubleSpinBox_koncanica_2->setRange(get_min_koncanica(), get_max_koncanica());
    doubleSpinBox_koncanica_2->setDecimals(2);       // Preciznost za decimalne brojeve

    // Povezivanje slidera i spinBox-a s varijablom
    connect(horizontalSlider_koncanica_2, &QSlider::valueChanged, this, [&]() {
        double newValue = get_min_koncanica() + (get_max_koncanica() - get_min_koncanica()) *
                          horizontalSlider_koncanica_2->value() / GRANULACIJA_KONCANICE;
        set_koncanica_2(newValue);
        doubleSpinBox_koncanica_2->blockSignals(true);
        doubleSpinBox_koncanica_2->setValue(get_koncanica_2());
        doubleSpinBox_koncanica_2->blockSignals(false);
        koncanicaOsvjezena();
    });

    connect(doubleSpinBox_koncanica_2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&](double newValue) {
        set_koncanica_2(newValue);
        int sliderValue = static_cast<int>((get_koncanica_2() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * GRANULACIJA_KONCANICE);
        horizontalSlider_koncanica_2->blockSignals(true);
        horizontalSlider_koncanica_2->setValue(sliderValue);
        horizontalSlider_koncanica_2->blockSignals(false);
        koncanicaOsvjezena();
    });

    // Dodavanje dinamičke oznake pored linije
    QCPItemText *label = new QCPItemText(qplot);
    // Postavi svojstva oznake
    label->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Poravnanje
    label->position->setType(QCPItemPosition::ptPlotCoords);       // Koordinate grafika
    label->setText("C2");                           // Postavi broj kao tekst
    label->setFont(QFont("Arial", 10));                           // Font i veličina

    labelica_koncanica2 = label;

}

// Ažuriranje granica pri promjeni min_koncanica ili max_koncanica
void Prikaz::azurirajGraniceKoncanice() {
    doubleSpinBox_koncanica->setRange(get_min_koncanica(), get_max_koncanica());
    double sliderValue = (get_koncanica_1() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * GRANULACIJA_KONCANICE;
    horizontalSlider_koncanica->setValue(static_cast<int>(sliderValue));

    doubleSpinBox_koncanica_2->setRange(get_min_koncanica(), get_max_koncanica());
    double sliderValue_2 = (get_koncanica_2() - get_min_koncanica()) / (get_max_koncanica() - get_min_koncanica()) * GRANULACIJA_KONCANICE;
    horizontalSlider_koncanica_2->setValue(static_cast<int>(sliderValue_2));
};



void Prikaz::osvjeziTabeluKoncanica()
{
    matricaSadrzajaTabelaKoncanica.clear();

    //dodamo jos i boju
    QString colorCrna = "#000000";

    std::vector<QString> kolona;
    kolona.push_back(" ");
    kolona.push_back("Cursor 1");
    kolona.push_back("Cursor 2");
    kolona.push_back("Difference");
    kolona.push_back("Gradient");
    kolona.push_back(colorCrna);
    matricaSadrzajaTabelaKoncanica.push_back(kolona);

    kolona.clear();
    kolona.push_back("Time [ms]");
    kolona.push_back(QString::number(get_koncanica_1(),'f', 2));
    kolona.push_back(QString::number(get_koncanica_2(),'f', 2));
    kolona.push_back(QString::number(get_koncanica_1()-get_koncanica_2(),'f', 2));
    kolona.push_back("  ");
    kolona.push_back(colorCrna);
    matricaSadrzajaTabelaKoncanica.push_back(kolona);


    for (auto signal: vektor_pSignala)
    {
        kolona.clear();

        QString imeSignala = signal->ime();
        //Originalne postavke
       // double vrij_signala_t1 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_1());
       // double vrij_signala_t2 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_2());
        //Sa vshiftom
        double vrij_signala_t1 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_1())+signal->get_signal_vshift();
        double vrij_signala_t2 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_2())+signal->get_signal_vshift();

        kolona.push_back(imeSignala);
        kolona.push_back(QString::number(vrij_signala_t1));
        kolona.push_back(QString::number(vrij_signala_t2));
        kolona.push_back(QString::number(vrij_signala_t1-vrij_signala_t2));
        //Izvod (mnozimo zbog milisekundi!)
        kolona.push_back(QString::number(1000.0*(vrij_signala_t1-vrij_signala_t2)/(get_koncanica_1()-get_koncanica_2())));
        //dodamo jos i boju
        QString colorString = signal->getBojaSignala().name();
        kolona.push_back(colorString);
        matricaSadrzajaTabelaKoncanica.push_back(kolona);

    }


    //Vertikalni layout

    tabelaTrenutnaKoncanica->clearContents();
    tabelaTrenutnaKoncanica->setRowCount(matricaSadrzajaTabelaKoncanica.size());
    tabelaTrenutnaKoncanica->setColumnCount(5);


    for (ulong i=0; i<matricaSadrzajaTabelaKoncanica.size(); i++){
        for (ulong j=0; j<matricaSadrzajaTabelaKoncanica[i].size()-1; j++)  //skracujemo za jedan a citamo boju iz posljednje kolone
        {
            QTableWidgetItem* item = tabelaTrenutnaKoncanica->item(i, j);
            if (!item) {
                item = new QTableWidgetItem();
                if (i>0) item->setTextAlignment(Qt::AlignRight);
                tabelaTrenutnaKoncanica->setItem(i, j, item);
            }

            //zaokruzivanje prikaza
            if ((i>=1)&&(j>=1)){
                item->setText(matricaSadrzajaTabelaKoncanica[i][j].left(8)); //ukljuceno zaokruzivanje prikaza na 8 karaktera sve osim headera
            }
                else {
                item->setText(matricaSadrzajaTabelaKoncanica[i][j]);
            }
            //farbanje u boju
            QColor boja = QColor(matricaSadrzajaTabelaKoncanica[i].back());
            item->setTextColor(boja);

        }
    }

    //Postavljanje na istu sirinu
    int columnCount = tabelaTrenutnaKoncanica->columnCount();
    for (int col = 0; col < columnCount; ++col) {
        tabelaTrenutnaKoncanica->setColumnWidth(col, 70);
    }

    tabelaTrenutnaKoncanica->verticalHeader()->setVisible(false);
    tabelaTrenutnaKoncanica->horizontalHeader()->setVisible(false);
    tabelaTrenutnaKoncanica->verticalHeader()->setDefaultSectionSize(25);

    //tabelaTrenutnaKoncanica->resizeColumnsToContents();

    /*
     * horizontalni layout
    matricaSadrzajaTabelaKoncanica.clear();

    std::vector<QString> kolona;
    kolona.push_back(" ");
    kolona.push_back("Cursor 1");
    kolona.push_back("Cursor 2");
    kolona.push_back("Difference");
    matricaSadrzajaTabelaKoncanica.push_back(kolona);

    kolona.clear();
    kolona.push_back("Time [ms]");
    kolona.push_back(QString::number(get_koncanica_1(),'f', 2));
    kolona.push_back(QString::number(get_koncanica_2(),'f', 2));
    kolona.push_back(QString::number(get_koncanica_1()-get_koncanica_2(),'f', 2));
    matricaSadrzajaTabelaKoncanica.push_back(kolona);


    for (auto signal: vektor_pSignala)
    {
        kolona.clear();

        QString imeSignala = signal->ime();
        //Originalne postavke
       // double vrij_signala_t1 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_1());
       // double vrij_signala_t2 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_2());
        //Sa vshiftom
        double vrij_signala_t1 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_1())+signal->get_signal_vshift();
        double vrij_signala_t2 = signal->vratiVrijednostSignalaUtrenutku(get_koncanica_2())+signal->get_signal_vshift();

        kolona.push_back(imeSignala);
        kolona.push_back(QString::number(vrij_signala_t1));
        kolona.push_back(QString::number(vrij_signala_t2));
        kolona.push_back(QString::number(vrij_signala_t1-vrij_signala_t2));
        matricaSadrzajaTabelaKoncanica.push_back(kolona);

    }


    tabelaTrenutnaKoncanica->clearContents();
    tabelaTrenutnaKoncanica->setRowCount(4);
    tabelaTrenutnaKoncanica->setColumnCount(matricaSadrzajaTabelaKoncanica.size());


    for (ulong i=0; i<matricaSadrzajaTabelaKoncanica.size(); i++){
        for (ulong j=0; j<matricaSadrzajaTabelaKoncanica[i].size(); j++)
        {
            QTableWidgetItem* item = tabelaTrenutnaKoncanica->item(j, i);
            if (!item) {
                item = new QTableWidgetItem();
                if (i>0) item->setTextAlignment(Qt::AlignRight);
                tabelaTrenutnaKoncanica->setItem(j, i, item);
            }
            item->setText(matricaSadrzajaTabelaKoncanica[i][j]);
        }
    }

    //Postavljanje na istu sirinu
    int columnCount = tabelaTrenutnaKoncanica->columnCount();
    for (int col = 0; col < columnCount; ++col) {
        tabelaTrenutnaKoncanica->setColumnWidth(col, 100);
    }

    tabelaTrenutnaKoncanica->verticalHeader()->setVisible(false);
    tabelaTrenutnaKoncanica->horizontalHeader()->setVisible(false);

    //tabelaTrenutnaKoncanica->resizeColumnsToContents();
    */

}

QString Prikaz::num2str(double value)
{
    int totalDigits = 7;    // Ukupan broj cifara
    int integerDigits = QString::number(static_cast<int>(value)).length(); // Cifre prije decimalne tačke
    int decimalDigits = qMax(0, totalDigits - integerDigits);              // Cifre poslije decimalne tačke

    QString formattedValue = QString::number(value, 'f', decimalDigits);

    return formattedValue;
}


//Ovo izbaciti u drugu klasu!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Prikaz::napraviSnapshotKoncanice(QString tekuciFajl)
{
    //Priprema excel fajla
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        tr("Save File"),
        "",
        tr("Excel Files (*.xls *.xlsx);;All Files (*)"));

    if (fileName.isEmpty()) {
        return; // User canceled the dialog
    }

    // Proverite da li fajl ima odgovarajuću ekstenziju
    if (!fileName.endsWith(".xls", Qt::CaseInsensitive) &&
        !fileName.endsWith(".xlsx", Qt::CaseInsensitive)) {
        fileName.append(".xlsx"); // Dodajte podrazumevanu ekstenziju
    }



    // Stvori Excel radnu knjigu
    QXlsx::Document workbook(fileName);

    workbook.addSheet("Snapshot");
    workbook.selectSheet("Snapshot");

    workbook.write(1, 1, "File name:");
    workbook.write(1, 2, tekuciFajl);

    workbook.write(2, 1, "Annotation:");
    QString annotation = getSnapshotAnnotation(nullptr);
    workbook.write(2, 2, annotation);

    int row = 4;

    // Definiši format sa promenjenom bojom fonta (crvena boja u ovom slučaju)
    QXlsx::Format format;

    for (ulong i=0; i<matricaSadrzajaTabelaKoncanica.size(); i++){
        //Izvuces boju sa posljednjeg reda
        QColor boja = QColor(QString::fromStdString(matricaSadrzajaTabelaKoncanica[i].back().toStdString()));
        for (ulong j=0; j<matricaSadrzajaTabelaKoncanica[i].size()-1; j++) //ide se do posljednjeg (posljednji je boja)
        {            
            format.setFontColor(boja); // Možeš koristiti i druge boje (npr. Qt::blue, Qt::green, itd.)
            workbook.write(row+j,i+1, matricaSadrzajaTabelaKoncanica[i][j],format);
        }
    }

    // Pretvori QCustomPlot u QPixmap
    QPixmap pixmap = qplot->toPixmap(800,800);
    // Sačuvajte sliku na disk
    workbook.insertImage(8,1,pixmap.toImage());


    // Spremi fajl
    if (workbook.saveAs(fileName)) {
        QMessageBox::information(nullptr, tr("File Saved"), tr("The file has been saved successfully."));
    } else {
        QMessageBox::warning(nullptr, tr("Save Failed"), tr("The file could not be saved."));
    }

    //Otvaranje fajla ako treba
    QUrl fileUrl = QUrl::fromLocalFile(fileName);
    QDesktopServices::openUrl(fileUrl);


}

QString Prikaz::getSnapshotAnnotation(QWidget *parent = nullptr) {
    bool ok;
    QString text = QInputDialog::getText(parent,
                                         "Snapshot annotation",
                                         "Enter snapshot annotation (optional):",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if (ok && !text.isEmpty()) {
        return text; // Vraća uneti tekst ako je potvrđeno
    }
    return QString(); // Vraća prazan string ako korisnik otkaže
}


void Prikaz::osvjeziElementeNaFormi()
{
    inicijalizirajKoncanicu();
    inicijalizirajKoncanicu_2();
    azurirajGraniceKoncanice();

    //Osvjezi jos boje dugmica - ovo se moze ugraditi u zajednicku funkciju
    pushButton_koncanica1_color->setStyleSheet(QString("background-color: %1; color: white;").arg(get_koncanica_1_color().name()));
    pushButton_koncanica2_color->setStyleSheet(QString("background-color: %1; color: white;").arg(get_koncanica_2_color().name()));

    koncanicaOsvjezena();
}




// Funkcija za inicijalizaciju
void Prikaz::setupPlot_zaInteraktivniPravougaonik()
{

    qplot->setInteraction(QCP::iRangeDrag, false);
    qplot->setInteraction(QCP::iRangeZoom , false);
    qplot->setInteraction(QCP::iSelectItems, false);

    // Inicijalizacija pravougaonika

    QCPItemRect* selRec = new QCPItemRect(qplot);
    selectionRect = selRec;
    selectionRect->setPen(QPen(Qt::DashLine));       // Isprekidana linija
    selectionRect->setBrush(QBrush(QColor(0, 0, 255, 50))); // Poluprozirna boja
    selectionRect->setVisible(false);               // Skriven do početka selekcije


    // Povezivanje događaja miša
    connect(qplot, &QCustomPlot::mousePress, this, &Prikaz::onMousePress);
    connect(qplot, &QCustomPlot::mouseMove, this, &Prikaz::onMouseMove);
    connect(qplot, &QCustomPlot::mouseRelease, this, &Prikaz::onMouseRelease);

}

// Funkcija za početak selekcije
void Prikaz::onMousePress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       // setupPlot_zaInteraktivniPravougaonik();

        startPoint = QPointF(
            qplot->xAxis->pixelToCoord(event->pos().x()),
            qplot->yAxis->pixelToCoord(event->pos().y())
        );
        selectionRect->topLeft->setCoords(startPoint);
        selectionRect->bottomRight->setCoords(startPoint); // Početni pravougaonik
        selectionRect->setVisible(true);                  // Pokaži pravougaonik
        qplot->replot();
    }
}

// Funkcija za ažuriranje pravougaonika tokom povlačenja
void Prikaz::onMouseMove(QMouseEvent *event)
{
    if (selectionRect->visible())
    {
        endPoint = QPointF(qplot->xAxis->pixelToCoord(event->pos().x()),
                           qplot->yAxis->pixelToCoord(event->pos().y()));
        selectionRect->bottomRight->setCoords(endPoint); // Postavi krajnje koordinate
        qplot->replot();
    }
}

// Funkcija za završetak selekcije
void Prikaz::onMouseRelease(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && selectionRect->visible())
    {
        // Finaliziraj pravougaonik
        endPoint = QPointF(qplot->xAxis->pixelToCoord(event->pos().x()),
                           qplot->yAxis->pixelToCoord(event->pos().y()));

        // Selektovane koordinate (početne i krajnje)
        //qDebug() << "Selection rectangle: Start =" << startPoint << ", End =" << endPoint;

        // Sakrij pravougaonik nakon akcije
        selectionRect->setVisible(false);
        qplot->replot();

        // Pozovi željenu akciju sa koordinatama
        handleSelection(startPoint, endPoint);
    }
}

// Funkcija za obradu selekcije
void Prikaz::handleSelection(QPointF start, QPointF end)
{
    // Implementiraj šta treba da se uradi sa selektovanim podacima
    //qDebug() << "Handling selection from" << start << "to" << end;
    if (tip_prikaza == "ul") {
        //Ako oznacis na malom promijeni initial time i duration u tekstboxovima
        lineEdit_initTime->setText(QString::number(start.x()));
        lineEdit_durationTime->setText(QString::number(end.x()-start.x()));
        // Simuliraj pritisak Enter tastera
        lineEdit_initTime->editingFinished();
        lineEdit_durationTime->editingFinished();

    } else if (tip_prikaza == "izl") {
        //Ako oznacis na glavnom  prikazi zoomani prozor
        showSelectedArea(start, end);
    }
}


void Prikaz::showSelectedArea(QPointF start, QPointF end)
{
    // Kreiraj novi dijalog ili widget
    QDialog *dialog = new QDialog(nullptr);
    dialog->setWindowTitle("Selected Area");
    dialog->resize(600, 600);
    // Postavi dijalog kao nemodalan
    dialog->setModal(false);
    // Automatski obriši dijalog kada se zatvori
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Kreiraj novi QCustomPlot
    QCustomPlot *plot = new QCustomPlot(dialog);
    plot->resize(dialog->size());
    //Opcionalno!!!!!!!!!!!!!!!!!!!!!!!!!!!
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // Kopiraj podatke sa originalnog grafika
    for (int i = 0; i < qplot->graphCount(); ++i)
    {
        QCPGraph *originalGraph = qplot->graph(i);
        QCPGraph *newGraph = plot->addGraph();

        // Kopiraj podatke iz originalnog grafa
        QSharedPointer<QCPGraphDataContainer> dataContainer = originalGraph->data();
        QSharedPointer<QCPGraphDataContainer> newContainer(new QCPGraphDataContainer);

        for (auto it = dataContainer->constBegin(); it != dataContainer->constEnd(); ++it)
        {
            // Dodaj samo podatke unutar selektovanog opsega
            if (it->key >= start.x() && it->key <= end.x())
            {
                newContainer->add(*it);
            }
        }

        newGraph->setData(newContainer);
        newGraph->setPen(originalGraph->pen()); // Kopiraj stil linije
    }

    // Postavi granice osa na selektovani deo
    plot->xAxis->setRange(start.x(), end.x());
    plot->yAxis->setRange(start.y(), end.y());

    // Dodaj QCustomPlot na dijalog i prikaži
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(plot);
    dialog->setLayout(layout);

    // Prikaz dijaloga
   // dialog->exec();
    dialog->show();
}

