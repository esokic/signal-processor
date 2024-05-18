#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ansamblsignala.h"
#include "manipulacijaprocesorima.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    AnsamblSignala ansamblSignala;
    pAnsamblSignala = &ansamblSignala;



    defaultniProcesor.setIme("---");

    prikaz1.setPointerQPlot(ui->mojCustomPlot1);
    prikaz2.setPointerQPlot(ui->mojCustomPlot2);

    manipulatorProc.show();




}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::citajIzMatFajla(const QString& filePath, AnsamblSignala*& ansamblSignala)
{

    // Pretvorite QString u char* jer MatIO koristi char*
    const char* filename = filePath.toLocal8Bit().constData();

    // Otvaranje .mat datoteke za čitanje
    mat_t *mat = Mat_Open(filename, MAT_ACC_RDONLY);
//    if (mat == NULL) {
//        qDebug() << "Nije moguće otvoriti datoteku";
//        return;
//    }

    matvar_t *matvar;
    while ((matvar = Mat_VarReadNext(mat)) != nullptr) {

        Signal* pSignal = new Signal;
        pSignal->ucitajSignalIzMatlabVarijable(matvar);

        pSignal->setPointerNaProcesor(&defaultniProcesor);

        ansamblSignala->dodajUAnsambl(pSignal);

        // Ne zaboravite osloboditi varijablu nakon što završite s njom
        Mat_VarFree(matvar);
    }

    //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
    ansamblSignala->dodijeliMarkerValueSvimSignalima();

    ansamblSignala->presloziVektorSignalaPoAbecedi();

    // Zatvaranje .mat datoteke
    Mat_Close(mat);

}

void MainWindow::on_pushButton_Refresh_clicked()
{

    //Postavi odabrani procesor kao procesor za odabrani signal
    signalUnderAnalysis->setPointerNaProcesor(manipulatorProc.getPointerNaOdabraniProcesor());

    //Sada procesiraj
    signalUnderAnalysis->procesirajSignal();

    //Osvjezavanje bi trebalo ici automatski ali nema veze
    prikaz2.osvjeziPrikaz();
}


void MainWindow::onTableWidgetItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if (current) {
        int row = current->row();
        Signal* selectedSignal = static_cast<Signal*>(ui->tableWidget_Signali->item(row, 0)->data(Qt::UserRole).value<void*>());

        Signal* ptest = selectedSignal;
        signalUnderAnalysis = ptest;

        prikaz1.setTipPrikaza("ul");
        prikaz1.ocistiPrikaz();
        prikaz1.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz1.osvjeziPrikaz();



        prikaz2.setTipPrikaza("izl");
        prikaz2.ocistiPrikaz();
        prikaz2.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz2.osvjeziPrikaz();

    }
}



void MainWindow::onListWidgetItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    if (current) {
        Signal *selectedSignal = static_cast<Signal*>(current->data(Qt::UserRole).value<void*>());
        // Sada možeš koristiti selectedSignal kako ti je potrebno

        Signal* ptest = selectedSignal;
        signalUnderAnalysis = ptest;

        prikaz1.setTipPrikaza("ul");
        prikaz1.ocistiPrikaz();
        prikaz1.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz1.osvjeziPrikaz();



        prikaz2.setTipPrikaza("izl");
        prikaz2.ocistiPrikaz();
        prikaz2.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz2.osvjeziPrikaz();

        /*
        if (manipulatorProc.getPointerNaOdabraniProcesor()!=nullptr)
        {
            manipulatorProc.getPointerNaOdabraniProcesor()->setPointerSignalUlazni1(test);
            manipulatorProc.getPointerNaOdabraniProcesor()->setPointerSignalIzlazni1(testout);
        }*/
        // OVO TREBA SREDITIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII - DA SE MOGU PRIDRUZIVATI POKAZIVACI I SL.
    }
}



void MainWindow::on_pushButton_importFile_clicked()
{
    /*
    QString executableDir = QCoreApplication::applicationDirPath();
    QString relativeFilePath = "../60.MAT";
    QString absoluteFilePath = QDir::cleanPath(executableDir + QDir::separator() + relativeFilePath);
    QString filePath = absoluteFilePath;
    */

    // Otvaranje dijaloškog prozora za odabir datoteke
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import File"), "", tr("MAT Files (*.MAT);;All Files (*)"));

    // Provjerite je li datoteka odabrana
    if (filePath.isEmpty()) {
        return; // Ako nije odabrana nijedna datoteka, izađite iz funkcije
    }




    //Ovo je malo varanje ali nemam druge varijante
    connect(pAnsamblSignala, &AnsamblSignala::changedMarkerValue, &manipulatorProc, &ManipulacijaProcesorima::onChangedMarkerValue);

    citajIzMatFajla(filePath, pAnsamblSignala);



    pAnsamblSignala->ispisiSveSignale();
    ui->label_nazivFajla->setText(filePath);

    /*
    //Popunjavanje tabele signala -------------------------
    //QListWidget *listWidget = new QListWidget(this);
    auto vektorSignala = ansamblSignala.dajVektorSignala();

    // Poredajte vektor signala po abecednom redu prema imenu
    std::sort(vektorSignala.begin(), vektorSignala.end(), [](Signal* a, Signal* b) {
        return a->ime() < b->ime();
    });

    // Očistite postojeće stavke u listWidgetu
    listWidget = ui->listWidget_Signali;
    listWidget->clear();

    // Dodajte poredane signale u listWidget
    for (auto &signal : vektorSignala) {
        QListWidgetItem *item = new QListWidgetItem(signal->ime());
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(signal)));
        listWidget->addItem(item);
    }

    // Povezivanje signala sa slotom
    connect(listWidget, &QListWidget::currentItemChanged, this, &MainWindow::onListWidgetItemChanged);
    */
    populateTableWidget_zaSignale(pAnsamblSignala);

}

void MainWindow::on_pushButton_exportFile_clicked()
{

}

void MainWindow::populateTableWidget_zaSignale(AnsamblSignala*& ansamblSignala)
{
    // Popunjavanje tabele signala
    auto vektorSignala = ansamblSignala->dajVektorSignala();

    /*
    // Poredajte vektor signala po abecednom redu prema imenu
    std::sort(vektorSignala.begin(), vektorSignala.end(), [](Signal* a, Signal* b) {
        return a->ime() < b->ime();
    });
    */

    // Očistite postojeće stavke u tableWidgetu
    QTableWidget *tableWidget = ui->tableWidget_Signali;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(5);
    //Ovo je za tabelu
    QStringList headers;
    headers << "Signal name" << "Processor name" << "Refresh time" << "New signal name" << "Export (Yes/No)";
    ui->tableWidget_Signali->setHorizontalHeaderLabels(headers);
    //Da se moze odabrati samo jedan red:
    ui->tableWidget_Signali->setSelectionMode(QAbstractItemView::SingleSelection);

    // Dodajte poredane signale u tableWidget
    for (size_t row = 0; row < vektorSignala.size(); ++row) {
        Signal* signal = vektorSignala[row];

        tableWidget->insertRow(static_cast<int>(row));

        // Ime signala
        QTableWidgetItem *nameItem = new QTableWidgetItem(signal->ime());
        tableWidget->setItem(static_cast<int>(row), 0, nameItem);

        // Oznaka procesora
        QTableWidgetItem *processorItem = new QTableWidgetItem(signal->getPointerNaProcesor()->getIme());
        processorItem->setFlags(processorItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 1, processorItem);

        // Datum i vrijeme osvježavanja
        QDateTime lastUpdate = signal->getPointerNaProcesor()->getLastUpdateTime();
        QTableWidgetItem *dateItem = new QTableWidgetItem(lastUpdate.toString(Qt::DefaultLocaleShortDate));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 2, dateItem);

        // "New name" editable field
        QPlainTextEdit *newNameEdit = new QPlainTextEdit(signal->getNewName());
        tableWidget->setCellWidget(static_cast<int>(row), 3, newNameEdit);

        // Checkbox for export
        QCheckBox *exportCheckBox = new QCheckBox();
        exportCheckBox->setChecked(signal->isMarkedForExport());
        tableWidget->setCellWidget(static_cast<int>(row), 4, exportCheckBox);

    }

    connect(ui->tableWidget_Signali, &QTableWidget::itemSelectionChanged, this, &MainWindow::onItemSelectionChanged);

}

void MainWindow::onItemSelectionChanged() {
    /*
    QTableWidgetItem *item = ui->tableWidget_tabelaProcesora->currentItem();
    if (item) {
        // Do something with the selected item
        qDebug() << "Selected item:" << item->text();
    }
    */

    // Dobijte listu odabranih indeksa u tabeli
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget_Signali->selectedItems();

    // Provjerite da li je izabran tačno jedan red
    if (selectedItems.size() != 1) {
        // Ako nije izabran tačno jedan red, poništite odabir i izađite iz metode
        ui->tableWidget_Signali->clearSelection();
        return;
    }



    // Dobijte redni broj izabranog reda
    int selectedRow = selectedItems[0]->row();

    // Provjerite da li je redni broj u granicama vektora
    if (selectedRow < 0 || selectedRow >= static_cast<int>(pAnsamblSignala->dajVektorSignala().size())) {
        // Ako nije, poništite odabir i izađite iz metode
        ui->tableWidget_Signali->clearSelection();
        return;
    }

    // Dobijte objekat Procesor iz vektora na osnovu rednog broja
    signalUnderAnalysis = pAnsamblSignala->dajVektorSignala()[static_cast<ulong>(selectedRow)];


    prikaz1.setTipPrikaza("ul");
    prikaz1.ocistiPrikaz();
    prikaz1.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
    prikaz1.osvjeziPrikaz();



    prikaz2.setTipPrikaza("izl");
    prikaz2.ocistiPrikaz();
    prikaz2.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
    prikaz2.osvjeziPrikaz();
}

