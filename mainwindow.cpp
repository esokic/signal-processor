#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ansamblsignala.h"
#include "manipulacijaprocesorima.h"
#include "signalsexport.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


   // defaultniProcesor.setIme("-");


    prikaz1.setPointerQPlot(ui->mojCustomPlot1);
    prikaz2.setPointerQPlot(ui->mojCustomPlot2);

}


MainWindow::~MainWindow()
{
    delete ui;
}

//Dio koji se tice manipulacije nad prikazima u QTableWidgetu
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


void MainWindow::populateTableWidget_zaSignale(AnsamblSignala*& ansamblSignala)
{
    // Očistite postojeće stavke u tableWidgetu
    QTableWidget *tableWidget = ui->tableWidget_Signali;
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(6);

    // Postavite zaglavlja
    QStringList headers;
    headers << "Show" << "Signal name" << "Processor name" << "Refresh time" << "New signal name" << "Export (Yes/No)";
    ui->tableWidget_Signali->setHorizontalHeaderLabels(headers);

    // Da se može odabrati samo jedan red
    ui->tableWidget_Signali->setSelectionMode(QAbstractItemView::SingleSelection);

    // Provjera da li je ansamblSignala validan
    if (ansamblSignala == nullptr || ansamblSignala->dajVektorSignalaSize() == 0) {
        return;  // Nema signala za prikaz
    }

    // Dodajte poredane signale u tableWidget
    for (size_t row = 0; row < ansamblSignala->dajVektorSignalaSize(); ++row) {
        Signal* signal = ansamblSignala->dajSignal(row);

        tableWidget->insertRow(static_cast<int>(row));

        // Checkbox for export
        QCheckBox *showCheckBox = new QCheckBox();
        showCheckBox->setChecked(signal->isMarkedForPrikaz());
        tableWidget->setCellWidget(static_cast<int>(row), 0, showCheckBox);
        connect(showCheckBox, &QCheckBox::toggled, [signal, showCheckBox]() {
            signal->setMarkedForExport(showCheckBox->isChecked());  // Postavljanje statusa prikaza na signal
        });

        // Ime signala
        QTableWidgetItem *nameItem = new QTableWidgetItem(signal->ime());
        tableWidget->setItem(static_cast<int>(row), 1, nameItem);

        // Oznaka procesora
        QTableWidgetItem *processorItem = new QTableWidgetItem(signal->getTrenutniProcesorIme());
        processorItem->setFlags(processorItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 2, processorItem);

        // Datum i vrijeme osvježavanja
        QDateTime lastUpdate = signal->getTrenutniProcesorUpdateTime();
        QTableWidgetItem *dateItem = new QTableWidgetItem(lastUpdate.toString(Qt::DefaultLocaleShortDate));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 3, dateItem);

        // "New name" editable field
        QPlainTextEdit *newNameEdit = new QPlainTextEdit(signal->getNewName());
        tableWidget->setCellWidget(static_cast<int>(row), 4, newNameEdit);
        connect(newNameEdit, &QPlainTextEdit::textChanged, [signal, newNameEdit]() {
            signal->set_novoIme(newNameEdit->toPlainText());  // Postavljanje novog imena na signal
        });

        // Checkbox for export
        QCheckBox *exportCheckBox = new QCheckBox();
        exportCheckBox->setChecked(signal->isMarkedForExport());
        tableWidget->setCellWidget(static_cast<int>(row), 5, exportCheckBox);
        connect(exportCheckBox, &QCheckBox::toggled, [signal, exportCheckBox]() {
            signal->setMarkedForExport(exportCheckBox->isChecked());  // Postavljanje statusa eksportovanja na signal
        });
    }

    // Povezivanje signala selekcije u tabeli sa odgovarajućim slotom
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
    if (selectedRow < 0 || selectedRow >= static_cast<int>(pAnsamblSignala->dajVektorSignalaSize())) {
        // Ako nije, poništite odabir i izađite iz metode
        ui->tableWidget_Signali->clearSelection();
        return;
    }

    // Dobijte objekat Procesor iz vektora na osnovu rednog broja
    signalUnderAnalysis = pAnsamblSignala->dajSignal(static_cast<ulong>(selectedRow));


    prikaz1.setTipPrikaza("ul");
    prikaz1.ocistiPrikaz();
    prikaz1.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
    prikaz1.osvjeziPrikaz();



    prikaz2.setTipPrikaza("izl");
    prikaz2.ocistiPrikaz();
    prikaz2.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
    prikaz2.osvjeziPrikaz();
}


//Callback nad dugmicima
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


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

    //Opcija brisanja prije ponovnog ucitavanja?
    pAnsamblSignala->ocisti();

    matvar_t *matvar;
    while ((matvar = Mat_VarReadNext(mat)) != nullptr) {

        //Signal* pSignal = new Signal; // std::make_unique<Signal>();
        auto pSignal = std::make_unique<Signal>();

        pSignal->ucitajSignalIzMatlabVarijable(matvar);

        //pSignal->setPointerNaProcesor(&defaultniProcesor);
        pSignal->setPointerNaProcesor(pManProc->getPointerNaDefaultniProcesor());

        //ansamblSignala->dodajUAnsambl(pSignal);
        ansamblSignala->dodajUAnsambl(std::move(pSignal));


        // Ne zaboravite osloboditi varijablu nakon što završite s njom
        Mat_VarFree(matvar);
    }

    //Ovo je malo varanje ali nemam druge varijante
     connect(pAnsamblSignala, &AnsamblSignala::changedMarkerValue, pManProc, &ManipulacijaProcesorima::onChangedMarkerValue);

    //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
    ansamblSignala->dodijeliMarkerValueSvimSignalima();

    ansamblSignala->presloziVektorSignalaPoAbecedi();

    // Zatvaranje .mat datoteke
    Mat_Close(mat);


    //IZMJESTITI OVO ODAVDE
    //Dio koji se tice osvjezavanja
    ansamblSignala->ispisiSveSignale();
    ui->label_nazivFajla->setText(filePath);

    //Ovaj dio je najkriticniji, trebalo bi da tableWidget bude na neki nacin neovisan od AnsamblaSignala
    populateTableWidget_zaSignale(ansamblSignala);

}

void MainWindow::on_pushButton_Refresh_clicked()
{

    //Postavi odabrani procesor kao procesor za odabrani signal
    signalUnderAnalysis->setPointerNaProcesor(pManProc->getPointerNaOdabraniProcesor());

    //Sada procesiraj
    signalUnderAnalysis->procesirajSignal();

    //Osvjezavanje bi trebalo ici automatski ali nema veze
    prikaz2.osvjeziPrikaz();
    populateTableWidget_zaSignale(pAnsamblSignala);
}



void MainWindow::on_pushButton_importFile_clicked()
{
    // Otvaranje dijaloškog prozora za odabir datoteke
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import File"), "", tr("MAT Files (*.MAT);;All Files (*)"));

    // Provjerite je li datoteka odabrana
    if (filePath.isEmpty()) {
        return; // Ako nije odabrana nijedna datoteka, izađite iz funkcije
    }

    citajIzMatFajla(filePath, pAnsamblSignala);

    //Ovo je jos da se omoguci klikanje lijevo desno za navigaciju po fajlovima
    QFileInfo fileInfo(filePath);
    folderPath = fileInfo.absolutePath();
    matFajlovi = dohvatiMatFajloveIzFoldera(folderPath);
    trenutniIndeks = matFajlovi.indexOf(fileInfo.fileName());



}


void MainWindow::on_pushButton_exportFile_clicked()
{
    pSigExp->show();
}



QStringList MainWindow::dohvatiMatFajloveIzFoldera(const QString& folderPath)
{
    QDir folder(folderPath);
    QStringList filters;
    filters << "*.mat"; // Filtriraj samo .mat fajlove
    return folder.entryList(filters, QDir::Files);
}


void MainWindow::ucitajNaredniFajl()
{
    // Ako folder nije prazan
    if (!matFajlovi.isEmpty()) {
        // Povećaj indeks za naredni fajl
        trenutniIndeks = (trenutniIndeks + 1) % matFajlovi.size();
        QString noviFajl = matFajlovi[trenutniIndeks];
        QString fullPath = folderPath + "/" + noviFajl;

        // Učitaj fajl
        citajIzMatFajla(fullPath, pAnsamblSignala);
    }
}

void MainWindow::ucitajPrethodniFajl()
{
    // Ako folder nije prazan
    if (!matFajlovi.isEmpty()) {
        // Povećaj indeks za prethodni fajl
        trenutniIndeks = (trenutniIndeks - 1 + matFajlovi.size()) % matFajlovi.size();
        QString noviFajl = matFajlovi[trenutniIndeks];
        QString fullPath = folderPath + "/" + noviFajl;

        // Učitaj fajl
        citajIzMatFajla(fullPath, pAnsamblSignala);
    }
}


void MainWindow::on_pushButton_prevFile_clicked()
{
    ucitajPrethodniFajl();
}

void MainWindow::on_pushButton_nextFile_clicked()
{
    ucitajNaredniFajl();
}
