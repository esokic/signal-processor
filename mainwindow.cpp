#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ansamblsignala.h"
#include "manipulacijaprocesorima.h"
#include "signalsexport.h"
#include "layout.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


   // defaultniProcesor.setIme("-");


    prikaz1.setPointerQPlot(ui->mojCustomPlot1);
    prikaz2.setPointerQPlot(ui->mojCustomPlot2);

    prikaz2.setPointerTableWidget_trenutnaKoncanica(ui->tableWidget_SnapshotKoncanica);
    //prikaz2.inicijaliziraj_tabelaTrenutnaKoncanica();

    prikaz2.setPointerQDoubleSpinBox(ui->doubleSpinBox_koncanica);
    prikaz2.setPointerQSlider(ui->horizontalSlider_koncanica);
    prikaz2.setPointerQDoubleSpinBox_2(ui->doubleSpinBox_koncanica_2);
    prikaz2.setPointerQSlider_2(ui->horizontalSlider_koncanica_2);
    prikaz2.setPointerQPushbutton_koncanica1_color(ui->pushButton_koncanica1_color);
    prikaz2.setPointerQPushbutton_koncanica2_color(ui->pushButton_koncanica2_color);
    prikaz2.inicijalizirajKoncanicu();
    prikaz2.inicijalizirajKoncanicu_2();


    prikaz1.setPointerQlineEdit_initTime(ui->lineEdit_initTime);
    prikaz1.setPointerQlineEdit_durationTime(ui->lineEdit_duration);

    //Poveznica prikaz1 i prikaz2 napravljena trenutno preko editboxa inittime i duration


    //Ovo izbaciti u odvojen dio -------------------------------
    // Time Init signala
    connect(ui->lineEdit_initTime, &QLineEdit::editingFinished, [this]() {
        this->prikaz2.set_initTime(ui->lineEdit_initTime->text().toDouble());  // Postavljanje nove pozicije signala
        //OVO VIDJETI DA LI JE OPTIMALNO DA SE NE DUPLIRA
        this->onOdabraniPrikazChanged();
    });

    // Time Duration signala
    connect(ui->lineEdit_duration, &QLineEdit::editingFinished, [this]() {
        this->prikaz2.set_durationTime(ui->lineEdit_duration->text().toDouble());  // Postavljanje nove pozicije signala
        //OVO VIDJETI DA LI JE OPTIMALNO DA SE NE DUPLIRA
        this->onOdabraniPrikazChanged();
    });
    //--------------------------------

    // Color koncanica 1
    QObject::connect(ui->pushButton_koncanica1_color, &QPushButton::clicked, [=]() {
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
        if (color.isValid()) {
            prikaz2.set_koncanica_1_color(color);
            ui->pushButton_koncanica1_color->setStyleSheet(QString("background-color: %1; color: white;").arg(color.name()));
            this->onOdabraniPrikazChanged();
        }
    });
    ui->pushButton_koncanica1_color->setStyleSheet(QString("background-color: %1; color: white;").arg(prikaz2.get_koncanica_1_color().name()));

    // Color koncanica 2
    QObject::connect(ui->pushButton_koncanica2_color, &QPushButton::clicked, [=]() {
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
        if (color.isValid()) {
            prikaz2.set_koncanica_2_color(color);
            ui->pushButton_koncanica2_color->setStyleSheet(QString("background-color: %1; color: white;").arg(color.name()));
            this->onOdabraniPrikazChanged();
        }
    });
    ui->pushButton_koncanica2_color->setStyleSheet(QString("background-color: %1; color: white;").arg(prikaz2.get_koncanica_2_color().name()));


    prikaz1.setupPlot_zaInteraktivniPravougaonik();
    prikaz2.setupPlot_zaInteraktivniPravougaonik();




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
    tableWidget->setColumnCount(8);

    // Postavite zaglavlja
    QStringList headers;
    headers << "Show" << "Signal name" << "Color" << "Position(%)" << "Size(%)" << "VShift" << "Export\nto Excel\n(Yes/No)" << "Applied Processor";// << "Refresh time" << "New signal name" << ;
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
        Signall* signal = ansamblSignala->dajSignal(row);

        tableWidget->insertRow(static_cast<int>(row));

        // 1. Checkbox for prikaz
        QCheckBox *showCheckBox = new QCheckBox();
        showCheckBox->setChecked(signal->isMarkedForPrikaz());
        // Centriranje
        QWidget *containerWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(containerWidget);
        layout->addWidget(showCheckBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        containerWidget->setLayout(layout);
        tableWidget->setCellWidget(static_cast<int>(row), 0, containerWidget);
        // -----------
        connect(showCheckBox, &QCheckBox::toggled, [signal, showCheckBox]() {
            signal->setMarkedForPrikaz(showCheckBox->isChecked());  // Postavljanje statusa prikaza na signal
        });
        connect(showCheckBox, &QCheckBox::toggled, this, &MainWindow::onOdabraniPrikazChanged);

        // 2. Ime signala
        QTableWidgetItem *nameItem = new QTableWidgetItem(signal->ime());
        tableWidget->setItem(static_cast<int>(row), 1, nameItem);

        // 3. Color
        QPushButton *colorButton = new QPushButton(" ");
        QObject::connect(colorButton, &QPushButton::clicked, [=]() {
            QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
            if (color.isValid()) {
                signal->setBojaSignala(color);
                colorButton->setStyleSheet(QString("background-color: %1; color: white;").arg(signal->getBojaSignala().name()));
                this->onOdabraniPrikazChanged();
            }
        });
        colorButton->setFixedWidth(30);
        colorButton->setFixedHeight(30);
        colorButton->setStyleSheet(QString("background-color: %1; color: white;").arg(signal->getBojaSignala().name()));
        tableWidget->setCellWidget(row, 2, colorButton);

        // 4. Position(%) signala
        QLineEdit *signalPositionEdit = new QLineEdit(QString::number(signal->get_signal_position()));
        tableWidget->setCellWidget(static_cast<int>(row), 3, signalPositionEdit);
        connect(signalPositionEdit, &QLineEdit::editingFinished, [signal, signalPositionEdit,this]() {
            signal->set_signal_position(signalPositionEdit->text().toInt());  // Postavljanje nove pozicije signala
            //OVO VIDJETI DA LI JE OPTIMALNO DA SE NE DUPLIRA
            this->onOdabraniPrikazChanged();
        });

        // 5. Size(%) signala
        QLineEdit *signalSizeEdit = new QLineEdit(QString::number(signal->get_signal_size()));
        tableWidget->setCellWidget(static_cast<int>(row), 4, signalSizeEdit);
        connect(signalSizeEdit, &QLineEdit::editingFinished, [signal, signalSizeEdit,this]() {
            signal->set_signal_size(signalSizeEdit->text().toDouble());  // Postavljanje novog size-a signala
            //OVO VIDJETI DA LI JE OPTIMALNO DA SE NE DUPLIRA
            this->onOdabraniPrikazChanged();
        });

        // 6. VSHIFT signala

        QLineEdit *signalVShiftEdit = new QLineEdit(QString::number(signal->get_signal_vshift()));
        tableWidget->setCellWidget(static_cast<int>(row), 5, signalVShiftEdit);
        connect(signalVShiftEdit, &QLineEdit::editingFinished, [signal, signalVShiftEdit,this]() {
            signal->set_signal_vshift(signalVShiftEdit->text().toDouble());  // Postavljanje novog size-a signala
            //OVO VIDJETI DA LI JE OPTIMALNO DA SE NE DUPLIRA
            this->onOdabraniPrikazChanged();
        });


        // 7. Checkbox for export
        QCheckBox *exportCheckBox = new QCheckBox();
        exportCheckBox->setChecked(signal->isMarkedForExport());
        // Centriranje
        QWidget *containerWidget2 = new QWidget();
        QHBoxLayout *layout2 = new QHBoxLayout(containerWidget2);
        layout2->addWidget(exportCheckBox);
        layout2->setAlignment(Qt::AlignCenter);
        layout2->setContentsMargins(0, 0, 0, 0);
        containerWidget2->setLayout(layout2);
        // -----------
        tableWidget->setCellWidget(static_cast<int>(row), 6, containerWidget2);
        connect(exportCheckBox, &QCheckBox::toggled, [signal, exportCheckBox]() {
            signal->setMarkedForExport(exportCheckBox->isChecked());  // Postavljanje statusa eksportovanja na signal
        });

        // 8. Oznaka procesora
        QTableWidgetItem *processorItem = new QTableWidgetItem(signal->getTrenutniProcesorIme());
        processorItem->setFlags(processorItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 7, processorItem);
/*
        // 6. Datum i vrijeme osvježavanja
        QDateTime lastUpdate = signal->getTrenutniProcesorUpdateTime();
        QTableWidgetItem *dateItem = new QTableWidgetItem(lastUpdate.toString(Qt::DefaultLocaleShortDate));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(static_cast<int>(row), 5, dateItem);

        // 7. "New name" editable field
        QLineEdit *newNameEdit = new QLineEdit(signal->getNewName());
        tableWidget->setCellWidget(static_cast<int>(row), 6, newNameEdit);
        connect(newNameEdit, &QLineEdit::editingFinished, [signal, newNameEdit]() {
            signal->set_novoIme(newNameEdit->text());  // Postavljanje novog imena na signal
        });
*/
        tableWidget->setRowHeight(row, 25);
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
        prikaz1.set_ptrAktivniSignal(signalUnderAnalysis);
    }

}




void MainWindow::onOdabraniPrikazChanged() {

    // Očisti prikaz za oba objekta
    prikaz1.setTipPrikaza("ul");
    prikaz1.ocistiPrikaz();
    prikaz2.setTipPrikaza("izl");
    prikaz2.ocistiPrikaz();

    // Dodaj signalima koji su označeni za prikaz u obe grupe
    for (ulong i = 0; i < pAnsamblSignala->dajVektorSignalaSize(); ++i) {
        Signall* signal = pAnsamblSignala->dajSignal(i);
        if (signal->isMarkedForPrikaz()) {
            // Dodaj signal u oba prikaza ako je označen
            prikaz1.dodajSignaluGrupuZaPrikaz(signal);
            prikaz2.dodajSignaluGrupuZaPrikaz(signal);
        }
    }

    // Osveži prikaze
  prikaz1.osvjeziPrikaz();
  prikaz2.osvjeziPrikaz();

  prikaz2.osvjeziTabeluKoncanica();
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

                //Signall* pSignal = new Signal; // std::make_unique<Signal>();
                auto pSignal = std::make_unique<Signall>();

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
                Signall* postojeciSignal = ansamblSignala->dajSignalPoImenu(pSignal->ime());
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

            setTrenutniFilePath(filePath);

            //IZMJESTITI OVO ODAVDE -------------------------------------------------------------------------
            //Dio koji se tice osvjezavanja
            ansamblSignala->ispisiSveSignale();
            ui->label_nazivFajla->setText(getTrenutniFilePath());

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

void MainWindow::on_pushButton_clicked()
{

    prikaz2.napraviSnapshotKoncanice(getTrenutniFilePath());
    //saveCustomPlotScreenshot(ui->mojCustomPlot2, "screenshot.png");
}


void MainWindow::saveCustomPlotScreenshot(QCustomPlot* customPlot, const QString& filePath) {
    if (!customPlot) return;

    // Pretvori QCustomPlot u QPixmap
    QPixmap pixmap = customPlot->toPixmap();

    // Sačuvaj pixmap u datoteku
    if (!pixmap.save(filePath)) {
        qDebug() << "Failed to save screenshot to" << filePath;
    } else {
        qDebug() << "Screenshot saved to" << filePath;
    }
    /*
    QPixmap pixmap;

                // Sačuvajte sliku na disk
                pixmap = qplot.toPixmap(580,400);
                xlxs_fajl.insertImage(15,1,pixmap.toImage());
                */
}

void MainWindow::on_pushButton_exportLayout_clicked()
{
    Layout layout;
    layout.setPtrAnsamblSignala(pAnsamblSignala);
    layout.setPrtPrikaz(&prikaz2);

    //---------------------------------------------------------------
    // Otvorite QFileDialog za odabir lokacije i imena fajla
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Layout Files (*.lt);;All Files (*)");

    // Provjera da li korisnik nije otkazao dijalog (empty string je rezultat ako se klikne Cancel)
    if (fileName.isEmpty()) {
        return;
    }

    // Provjerite da li ime fajla već ima ekstenziju ".lt"
    if (!fileName.endsWith(".lt", Qt::CaseInsensitive)) {
        // Ako nema, dodajte ekstenziju
        fileName.append(".lt");
    }

    //---------------------------------------------------------------
    layout.export_to_file(fileName.toStdString());
}

void MainWindow::on_pushButton_importLayout_clicked()
{
    Layout loaded_layout;
    loaded_layout.setPtrAnsamblSignala(pAnsamblSignala);
    loaded_layout.setPrtPrikaz(&prikaz2);

    //----------------------------------------------------------------
    // Otvorite QFileDialog za odabir fajla
    QString fileName = QFileDialog::getOpenFileName(this, "Open Layout File", "", "Layout Files (*.lt);;All Files (*)");

    // Provjera da li korisnik nije otkazao dijalog (empty string je rezultat ako se klikne Cancel)
    if (fileName.isEmpty()) {
        return ;
    }

    // Provjerite da li ime fajla već ima ekstenziju ".lt"
    if (!fileName.endsWith(".lt", Qt::CaseInsensitive)) {
        // Ako nema, dodajte ekstenziju
        fileName.append(".lt");
    }

    //_---------------------------------------------------------------
    loaded_layout.import_from_file(fileName.toStdString());

    //I ovo osvjezavanje isto!!!!!!!!!!!!!!!!!!!!!!
    populateTableWidget_zaSignale(pAnsamblSignala);
    onItemSelectionChanged();
    onOdabraniPrikazChanged();

    prikaz2.osvjeziElementeNaFormi();

    ui->lineEdit_initTime->setText(QString::number(prikaz2.get_initTime()));
    ui->lineEdit_duration->setText(QString::number(prikaz2.get_durationTime()));

}


void MainWindow::on_pushButton_importDefaultLayout_clicked()
{
    Layout loaded_layout;
    loaded_layout.setPtrAnsamblSignala(pAnsamblSignala);
    loaded_layout.setPrtPrikaz(&prikaz2);
    loaded_layout.import_from_file("default_layout.lt");

    //I ovo osvjezavanje isto!!!!!!!!!!!!!!!!!!!!!!
    populateTableWidget_zaSignale(pAnsamblSignala);
    onItemSelectionChanged();
    onOdabraniPrikazChanged();

    prikaz2.osvjeziElementeNaFormi();

    ui->lineEdit_initTime->setText(QString::number(prikaz2.get_initTime()));
    ui->lineEdit_duration->setText(QString::number(prikaz2.get_durationTime()));
}

void MainWindow::on_pushButton_exportDefaultLayout_clicked()
{
    Layout layout;
    layout.setPtrAnsamblSignala(pAnsamblSignala);
    layout.setPrtPrikaz(&prikaz2);

    layout.export_to_file("default_layout.lt");
}




