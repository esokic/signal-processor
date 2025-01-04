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
    headers << "Show" << "Signal name" << "Processor name" << "Refresh time" << "New signal name" << "Export to Excel (Yes/No)";
    ui->tableWidget_Signali->setHorizontalHeaderLabels(headers);
    ui->tableWidget_Signali->resizeColumnsToContents();

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

        // Checkbox for prikaz
        QCheckBox *showCheckBox = new QCheckBox();
        showCheckBox->setChecked(signal->isMarkedForPrikaz());
        tableWidget->setCellWidget(static_cast<int>(row), 0, showCheckBox);
        connect(showCheckBox, &QCheckBox::toggled, [signal, showCheckBox]() {
            signal->setMarkedForPrikaz(showCheckBox->isChecked());  // Postavljanje statusa prikaza na signal
        });
        connect(showCheckBox, &QCheckBox::toggled, this, &MainWindow::onOdabraniPrikazChanged);

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

    // Dobijte listu odabranih indeksa u tabeli
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget_Signali->selectedItems();

    /*
    // Provjerite da li je izabran tačno jedan red
    if (selectedItems.size() != 1) {
        // Ako nije izabran tačno jedan red, poništite odabir i izađite iz metode
        ui->tableWidget_Signali->clearSelection();
        return;
    }
    */

    if (selectedItems.size() > 0)
    {
        // Dobijte redni broj izabranog reda - uzima samo prvi
        int selectedRow = selectedItems[0]->row();

        // Dobijte objekat Procesor iz vektora na osnovu rednog broja
        signalUnderAnalysis = pAnsamblSignala->dajSignal(static_cast<ulong>(selectedRow));
    }

}



//Stara verzija koja je omogucavala promjenu prikaza (za selektovanje i crtanje samo jednog dijagrama)
void MainWindow::onOdabraniPrikazChanged() {

    // Očisti prikaz za oba objekta
    prikaz1.setTipPrikaza("ul");
    prikaz1.ocistiPrikaz();
    prikaz2.setTipPrikaza("izl");
    prikaz2.ocistiPrikaz();

    // Dodaj signalima koji su označeni za prikaz u obe grupe
    for (ulong i = 0; i < pAnsamblSignala->dajVektorSignalaSize(); ++i) {
        Signal* signal = pAnsamblSignala->dajSignal(i);
        if (signal->isMarkedForPrikaz()) {
            // Dodaj signal u oba prikaza ako je označen
            prikaz1.dodajSignaluGrupuZaPrikaz(signal);
            prikaz2.dodajSignaluGrupuZaPrikaz(signal);
        }
    }

    // Osveži prikaze
  prikaz1.osvjeziPrikaz();
  prikaz2.osvjeziPrikaz();
}


//Callback nad dugmicima
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------


void MainWindow::citajIzMatFajla(const QString& filePath, AnsamblSignala*& ansamblSignala)
{
    // Kreiraj modalni dijalog za prikaz poruke "Loading..."
        QDialog loadingDialog(this);
        loadingDialog.setWindowTitle("Please Wait");
        loadingDialog.setModal(true);

        QVBoxLayout layout(&loadingDialog);
        QLabel label("Loading...");
        layout.addWidget(&label);

        // Postavi veličinu i prikaži dijalog
        loadingDialog.resize(200, 100);
        loadingDialog.show();

        // Koristi timer za pokretanje dugotrajnog zadatka u sledećem događaju glavne petlje
        QTimer::singleShot(100, [&]() {

            // Pretvorite QString u char* jer MatIO koristi char*
            const char* filename = filePath.toLocal8Bit().constData();

            // Otvaranje .mat datoteke za čitanje
            mat_t *mat = Mat_Open(filename, MAT_ACC_RDONLY);
            //    if (mat == NULL) {
            //        qDebug() << "Nije moguće otvoriti datoteku";
            //        return;
            //    }

            //Opcija brisanja prije ponovnog ucitavanja?
            //pAnsamblSignala->ocisti();
            QStringList listaNaziva; //Pravi listu svih signala u novoucitanom fajlu

            matvar_t *matvar;
            while ((matvar = Mat_VarReadNext(mat)) != nullptr) {

                //Signal* pSignal = new Signal; // std::make_unique<Signal>();
                auto pSignal = std::make_unique<Signal>();

                //ucitano iz fajla
                pSignal->ucitajSignalIzMatlabVarijable(matvar);
                pSignal->setPointerNaProcesor(pManProc->getPointerNaDefaultniProcesor());

                // Dodaj ime u listu ako nije prazno
                QString ime = pSignal->ime();
                if (!ime.isEmpty()) {
                    listaNaziva.append(ime);
                }

                // Provjera da li signal sa istim imenom već postoji, ako postoji kopiraj podesenja od interesa
                //ovo bi trebalo da bude opcija (ako zelis)
                //postojeci signal je mozda signal sa istim imenom u ansamblu -
                //OVA METODOLOGIJA NAZALOST SAMO DODAJE NOVE SIGNALE (moze ucitati "lazne") pa su dole iskljuceni
                Signal* postojeciSignal = ansamblSignala->dajSignalPoImenu(pSignal->ime());
                if (postojeciSignal) {

                    //ovdje se pravi fuzija - podaci iz pSignala su novi, a postojeciSignal stari (ima i lokacija)
                    //MERGE - izdvoji u odvojenu funkciju ------------------------------------------------------------
                    postojeciSignal->set_xData_izl(pSignal->get_xData_izl());
                    postojeciSignal->set_xData_ul(pSignal->get_xData_ul());
                    postojeciSignal->set_yData_izl(pSignal->get_yData_izl());
                    postojeciSignal->set_yData_ul(pSignal->get_yData_ul());
                    //postojeciSignal->setMarkerValue(pSignal->getMarkerValue());
                    //ostalo je jos podesenje da je marker podesen?
                    //OVERWRITE?
                    //------------------------------------------------------------

                } else {
                    ansamblSignala->dodajUAnsambl(std::move(pSignal));
                }

                // Ne zaboravite osloboditi varijablu nakon što završite s njom
                Mat_VarFree(matvar);
            }

            //Sad pocisti signale koji nisu u listi naziva
            pAnsamblSignala->ocistiVisakSignala(listaNaziva);


            //Ovo je malo varanje ali nemam druge varijante
             connect(pAnsamblSignala, &AnsamblSignala::changedMarkerValue, pManProc, &ManipulacijaProcesorima::onChangedMarkerValue);

            //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
            ansamblSignala->dodijeliMarkerValueSvimSignalima();

            ansamblSignala->presloziVektorSignalaPoAbecedi();

            // Zatvaranje .mat datoteke
            Mat_Close(mat);


            //IZMJESTITI OVO ODAVDE -------------------------------------------------------------------------
            //Dio koji se tice osvjezavanja
            ansamblSignala->ispisiSveSignale();
            ui->label_nazivFajla->setText(filePath);

            //Ovaj dio je najkriticniji, trebalo bi da tableWidget bude na neki nacin neovisan od AnsamblaSignala
            populateTableWidget_zaSignale(ansamblSignala);

            //I ovo osvjezavanje isto!!!!!!!!!!!!!!!!!!!!!!
            onItemSelectionChanged();
            onOdabraniPrikazChanged();
            //-------------------------------------------------------------------------


            // Zatvori dijalog nakon završetka zadatka
                loadingDialog.accept();
            });

        // Prikazuj dijalog dok se zadatak izvršava
        loadingDialog.exec();

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

void MainWindow::on_pushButton_exportExcel_clicked()
{
    pSigExp->exportFileExcel();
}
