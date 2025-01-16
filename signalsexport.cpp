#include "signalsexport.h"
#include "ui_signalsexport.h"
#include "exportfilesetting.h"
#include "selectsignalsdialog.h"
#include <QItemSelection>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>
#include <xlsxdocument.h>

SignalsExport::SignalsExport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalsExport)
{
    ui->setupUi(this);

    ui->tableWidget_ExportFileSettings->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_ExportFileSettings->setSelectionMode(QAbstractItemView::SingleSelection);

    // Connect signal for table widget item selection
    connect(ui->tableWidget_ExportFileSettings->selectionModel(), &QItemSelectionModel::currentRowChanged,
                    this, &SignalsExport::onTableWidgetSelectionChanged);

    // Povežite signal itemSelectionChanged sa slotom onTableWidgetSettingsSelectionChanged
    connect(ui->tableWidget_ExportFileSettings->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SignalsExport::onTableWidgetSettingsSelectionChanged);

    // Inicijalno postavite groupBox na disabled
    ui->groupBox->setEnabled(false);
}

SignalsExport::~SignalsExport()
{
    delete ui;
}

// Function to add a new ExportFileSetting object
void SignalsExport::addNewExportFileSetting(const QString& fileDescription, const QString& fileHeader, const QString& nazivFileSettingsa,
                                 const QString& nrPhases, const QString& exportFileType) {
    auto *newSetting = new ExportFileSetting();
    newSetting->setFileDescription(fileDescription);
    newSetting->setFileHeader(fileHeader);
    newSetting->setNazivFileSettingsa(nazivFileSettingsa);
    newSetting->setNrPhases(nrPhases);
    newSetting->setExportFileType(exportFileType);

    // Add the new setting to the vector
    vektorFileSettingsa.push_back(newSetting);

    // Refresh the table widget
    populateTableWidget();

}


// Function to populate the table widget
void SignalsExport::populateTableWidget() {

    ui->tableWidget_ExportFileSettings->clearContents();
    ui->tableWidget_ExportFileSettings->setRowCount(static_cast<int>(vektorFileSettingsa.size()));
    ui->tableWidget_ExportFileSettings->setColumnCount(3);

    QStringList headers;
    headers << "File settings name" << "nr.Phases" << "FileType";
    ui->tableWidget_ExportFileSettings->setHorizontalHeaderLabels(headers);
    //Da se moze odabrati samo jedan red:
    ui->tableWidget_ExportFileSettings->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < vektorFileSettingsa.size(); ++i) {
        ExportFileSetting *setting = vektorFileSettingsa[i];

       // QTableWidgetItem *itemFileDescription = new QTableWidgetItem(setting->getFileDescription());
       // QTableWidgetItem *itemFileHeader = new QTableWidgetItem(setting->getFileHeader());
        QTableWidgetItem *itemNazivFileSettingsa = new QTableWidgetItem(setting->getNazivFileSettingsa());
        QTableWidgetItem *itemNrPhases = new QTableWidgetItem(setting->getNrPhases());
        QTableWidgetItem *itemExportFileType = new QTableWidgetItem(setting->getExportFileType());

     //   ui->tableWidget_ExportFileSettings->setItem(static_cast<int>(i), 0, itemFileDescription);
     //   ui->tableWidget_ExportFileSettings->setItem(static_cast<int>(i), 1, itemFileHeader);
        ui->tableWidget_ExportFileSettings->setItem(static_cast<int>(i), 0, itemNazivFileSettingsa);
        ui->tableWidget_ExportFileSettings->setItem(static_cast<int>(i), 1, itemNrPhases);
        ui->tableWidget_ExportFileSettings->setItem(static_cast<int>(i), 2, itemExportFileType);

        // Postavljanje needitabilnih kolona
        itemNazivFileSettingsa->setFlags(itemNazivFileSettingsa->flags() & ~Qt::ItemIsEditable);
        itemNrPhases->setFlags(itemNrPhases->flags() & ~Qt::ItemIsEditable);
        itemExportFileType->setFlags(itemExportFileType->flags() & ~Qt::ItemIsEditable);

    }
}

// Slot to handle table widget selection change
void SignalsExport::onTableWidgetSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous)
    int row = current.row();
    if (row >= 0 && row < static_cast<int>(vektorFileSettingsa.size())) {
        ExportFileSetting *selectedSetting = vektorFileSettingsa[static_cast<size_t>(row)];
        updateUIFromVariables(*selectedSetting);
    }
}

void SignalsExport::onTableWidgetSettingsSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // Proverite da li je nešto selektovano
    bool hasSelection = !ui->tableWidget_ExportFileSettings->selectionModel()->selectedRows().isEmpty();

    // Postavite enabled stanje groupBox-a u zavisnosti od selekcije
    ui->groupBox->setEnabled(hasSelection);
}

// Funkcija za postavljanje varijabli prema stanju UI elemenata
void SignalsExport::updateVariablesFromUI(ExportFileSetting& settings) {
    settings.setFileDescription(ui->lineEdit_fileDescription->text());
    settings.setFileHeader(ui->lineEdit_fileHeader->text());
    settings.setNazivFileSettingsa(ui->lineEdit_nazivFileSettingsa->text());

    if (ui->radioButton_1phase->isChecked()) {
        settings.setNrPhases("1ph");
    } else if (ui->radioButton_3phase->isChecked()) {
        settings.setNrPhases("3ph");
    }

    if (ui->radioButton_fileTypeInput->isChecked()) {
        settings.setExportFileType("input");
    } else if (ui->radioButton_fileTypeOutput->isChecked()) {
        settings.setExportFileType("output");
    }
}

// Funkcija za postavljanje UI elemenata prema vrijednostima varijabli
void SignalsExport::updateUIFromVariables(ExportFileSetting& settings){
    ui->lineEdit_fileDescription->setText(settings.getFileDescription());
    ui->lineEdit_fileHeader->setText(settings.getFileHeader());
    ui->lineEdit_nazivFileSettingsa->setText(settings.getNazivFileSettingsa());

    if (settings.getNrPhases() == "1ph") {
        ui->radioButton_1phase->setChecked(true);
    } else if (settings.getNrPhases() == "3ph") {
        ui->radioButton_3phase->setChecked(true);
    }

    if (settings.getExportFileType() == "input") {
        ui->radioButton_fileTypeInput->setChecked(true);
    } else if (settings.getExportFileType() == "output") {
        ui->radioButton_fileTypeOutput->setChecked(true);
    }

    populateSignalsTableWidget(&settings);
}

void SignalsExport::on_pushButton_newFileExportSettings_clicked()
{
    int rowCount = ui->tableWidget_ExportFileSettings->rowCount();
    addNewExportFileSetting("(description)", "Travel, current and arc voltage", "(file setting ...)", "1ph", "input");
    ui->tableWidget_listaSignalaZaExport->clear();
    ui->tableWidget_ExportFileSettings->selectRow(rowCount+1);

}

void SignalsExport::on_pushButton_updateSettings_clicked()
{
    // Dobijanje trenutno selektovanog reda
    int currentRow = ui->tableWidget_ExportFileSettings->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(vektorFileSettingsa.size())) {
        ExportFileSetting *currentSetting = vektorFileSettingsa[static_cast<size_t>(currentRow)];

        // Ažuriranje objekta sa podacima iz UI elemenata
        updateVariablesFromUI(*currentSetting);

        // Osvježavanje tabele nakon ažuriranja
        populateTableWidget();
        populateSignalsTableWidget(currentSetting);
    }
    ui->tableWidget_ExportFileSettings->selectRow(currentRow);
}

/*
void SignalsExport::populateSignalsTableWidget(ExportFileSetting *currentSetting)
{
    // Osvježi tableWidget_listaSignalaZaExport
    ui->tableWidget_listaSignalaZaExport->setRowCount(currentSetting->getListaSignalaZaExport().size());
    ui->tableWidget_listaSignalaZaExport->setColumnCount(1);
    for (size_t i = 0; i < currentSetting->getListaSignalaZaExport().size(); ++i) {
        ui->tableWidget_listaSignalaZaExport->setItem(i, 0, new QTableWidgetItem(currentSetting->getListaSignalaZaExport()[i]->ime()));
       // ui->tableWidget_listaSignalaZaExport->setItem(i, 1, new QTableWidgetItem("N/A")); // Ovdje možete dodati ime procesora
       // ui->tableWidget_listaSignalaZaExport->setItem(i, 2, new QTableWidgetItem("N/A")); // Ovdje možete dodati datum i vrijeme osvježavanja
       // ui->tableWidget_listaSignalaZaExport->setItem(i, 3, new QTableWidgetItem("New name")); // Ovo je placeholder
       // ui->tableWidget_listaSignalaZaExport->setCellWidget(i, 4, new QCheckBox(this));
    }

}
*/
void SignalsExport::populateSignalsTableWidget(ExportFileSetting *currentSetting)
{
    // Osvježi tableWidget_listaSignalaZaExport
    ui->tableWidget_listaSignalaZaExport->setRowCount(currentSetting->getListaSignalaZaExport().size());
    ui->tableWidget_listaSignalaZaExport->setColumnCount(1);
    for (size_t i = 0; i < currentSetting->getListaSignalaZaExport().size(); ++i) {
        ui->tableWidget_listaSignalaZaExport->setItem(i, 0, new QTableWidgetItem(currentSetting->getListaSignalaZaExport()[i]));
    }

}

void SignalsExport::on_pushButton_addSignalToFile_clicked()
{
    int currentRow = ui->tableWidget_ExportFileSettings->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(vektorFileSettingsa.size())) {
        ExportFileSetting *currentSetting = vektorFileSettingsa[static_cast<size_t>(currentRow)];


        std::vector<QString> signalNames;
        for (int i=0; i< pAnsamblSignala->dajVektorSignalaSize(); i++) {
            signalNames.push_back(pAnsamblSignala->dajSignal(i)->ime());  // Dodajte ime signala u listu
        }

        SelectSignalsDialog dialog(signalNames, this);
        if (dialog.exec() == QDialog::Accepted) {
            std::vector<QString> selectedNames = dialog.getSelectedSignalNames();
            // Ovdje možete obraditi selektovane nazive
            for (const auto& signal : selectedNames) {
                currentSetting->dodajUListuSignalaZaExport(signal);
            }

                populateSignalsTableWidget(currentSetting);

            }

            // Osvježavanje tabele nakon ažuriranja
            populateTableWidget();

            ui->tableWidget_ExportFileSettings->selectRow(currentRow);

        }


    //PRIVREMENO ZAKOMENTARISANO DOK NE POPRAVIMO
    /*
    // Dobijanje trenutno selektovanog reda
    int currentRow = ui->tableWidget_ExportFileSettings->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(vektorFileSettingsa.size())) {
        ExportFileSetting *currentSetting = vektorFileSettingsa[static_cast<size_t>(currentRow)];

        // Otvori dijalog za odabir signala

        SelectSignalsDialog dialog(pAnsamblSignala->dajVektorSignala(), this);
        if (dialog.exec() == QDialog::Accepted) {
            // Dodaj odabrane signale u listaSignalaZaExport
            auto selectedSignals = dialog.getSelectedSignals();
            for (const auto& signal : selectedSignals) {
                currentSetting->dodajUListuSignalaZaExport(signal);
            }

            populateSignalsTableWidget(currentSetting);

        }

        // Osvježavanje tabele nakon ažuriranja
        populateTableWidget();

        ui->tableWidget_ExportFileSettings->selectRow(currentRow);

    }
    */


}

ExportFileSetting* SignalsExport::getCurrentFileSettings()
{
    ExportFileSetting *currentSetting = nullptr;
    //Uzimamo trenutno selektovan settings
    int currentRow = ui->tableWidget_ExportFileSettings->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(vektorFileSettingsa.size())) {
        currentSetting = vektorFileSettingsa[static_cast<size_t>(currentRow)];
    }
    return currentSetting;
}


void SignalsExport::on_pushButton_removeSignalToFile_clicked()
{
    //Uzimamo trenutno selektovan settings
    ExportFileSetting *currentSetting = getCurrentFileSettings();
    if (currentSetting==nullptr) return;

    // Dobijanje trenutno selektovanog reda signala
    int currentRow = ui->tableWidget_listaSignalaZaExport->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(currentSetting->getListaSignalaZaExport().size())) {
        //To znaci da smo selektovali nesto u listi signala za eksport
        currentSetting->izbrisiElementIzVektoraSignalaZaEksport(currentRow);
        //repopuliraj tabelu
        populateSignalsTableWidget(currentSetting);
    }

    // Osvježavanje tabele nakon ažuriranja
    //populateTableWidget();
    //ui->tableWidget_ExportFileSettings->selectRow(currentRow);

}

void SignalsExport::on_pushButton_signalUpList_clicked()
{
    //Uzimamo trenutno selektovan settings
    ExportFileSetting *currentSetting = getCurrentFileSettings();
    if (currentSetting==nullptr) return;

    // Dobijanje trenutno selektovanog reda signala
    int currentRow = ui->tableWidget_listaSignalaZaExport->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(currentSetting->getListaSignalaZaExport().size())) {
        //To znaci da smo selektovali nesto u listi signala za eksport
        currentSetting->shiftUpElementIzVektoraSignalaZaEksport(currentRow);
        //repopuliraj tabelu
        populateSignalsTableWidget(currentSetting);
    }

    // Osvježavanje tabele nakon ažuriranja
    //populateTableWidget();
    //ui->tableWidget_listaSignalaZaExport->selectRow(currentRow);
}

void SignalsExport::on_pushButton_signalDownList_clicked()
{
    //Uzimamo trenutno selektovan settings
    ExportFileSetting *currentSetting = getCurrentFileSettings();
    if (currentSetting==nullptr) return;

    // Dobijanje trenutno selektovanog reda signala
    int currentRow = ui->tableWidget_listaSignalaZaExport->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(currentSetting->getListaSignalaZaExport().size())) {
        //To znaci da smo selektovali nesto u listi signala za eksport
        currentSetting->shiftDownElementIzVektoraSignalaZaEksport(currentRow);
        //repopuliraj tabelu
        populateSignalsTableWidget(currentSetting);
    }

    // Osvježavanje tabele nakon ažuriranja
    //populateTableWidget();
   // ui->tableWidget_ExportFileSettings->selectRow(currentRow);
}

void SignalsExport::on_pushButton_exportFiles_clicked()
{
    ExportFileSetting *currentSetting = getCurrentFileSettings();
    if (currentSetting==nullptr)
    {
        //vrati gresku da nije nista selektovano za eksport
    } else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);All Files (*)"));

        if (fileName.isEmpty()) {
            return; // User canceled the dialog
        }

        if (currentSetting->getExportFileType() == "input") generateOutputHVCBFile_input(fileName, currentSetting);
        if (currentSetting->getExportFileType() == "output") generateOutputHVCBFile_output(fileName, currentSetting);

        //Otvaranje fajla ako treba

        #if defined(Q_OS_WIN)
            QProcess::startDetached("notepad.exe", QStringList() << fileName);
        #elif defined(Q_OS_LINUX)
            QProcess::startDetached("gedit", QStringList() << fileName);
        #endif

    }
}

//Privremeno
void SignalsExport::exportFileExcel()
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



        generateOutputExcelFile(fileName);

        //Otvaranje fajla ako treba
        QUrl fileUrl = QUrl::fromLocalFile(fileName);
        QDesktopServices::openUrl(fileUrl);

}


//Ovo je trenutno funkcija za eksportovanje u HVCB - input formatu
void SignalsExport::generateOutputHVCBFile_output(const QString& fileName, ExportFileSetting* setting) {
    std::ofstream outFile(fileName.toStdString());

    if (!outFile.is_open()) {
        // Handle error
        return;
    }

    // Provera da li ima signala za eksport
    if (setting->getListaSignalaZaExport().empty()) {
        //std::cerr << "No signals to export";
        return;
    }

    // 1. Header
    outFile << "\"" +setting->getFileHeader().toStdString() + "\"" << "\n";

    // 2. Description
    outFile << "\""+ setting->getFileDescription().toStdString() + "\"" << "\n";

    // 3. Some hard-coded value (e.g., 1001)

    outFile << QString::number(pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[0])->get_xData_izl().size()).toStdString() + "\n";

    // 4. Number of phases
    outFile << (setting->getNrPhases() == "1ph" ? "1\n" : "3\n");

    // Dodajte dodatne linije potrebne za formatiranje izlaznog fajla


   // Broj redova za ispis
   size_t rowCount = pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport().front())->get_xData_izl().size();

   // Ispis podataka
   for (size_t row = 0; row < rowCount; ++row) {
       // Prva kolona je xDataIzl, samo prvi signal
       outFile << pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport().front())->get_xData_izl(row);

       // Ostale kolone su yDataIzl za svaki signal
       for (ulong i=0; i<setting->getListaSignalaZaExport().size(); i++)
       {
            outFile << "\t" << pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[i])->get_yData_izl(row);
        }

       outFile << "\n";
   }

   outFile.close();
}

void SignalsExport::generateOutputHVCBFile_input(const QString& fileName, ExportFileSetting* setting)
{
    std::ofstream outFile(fileName.toStdString());

    if (!outFile.is_open()) {
        // Handle error
        return;
    }

    // Provera da li ima signala za eksport
    if (setting->getListaSignalaZaExport().empty()) {
        //std::cerr << "No signals to export";
        return;
    }

    // 1. Header
    outFile << "\"Results\"" << "\n";

    // 2. Broj
    outFile << setting->getListaSignalaZaExport().size() << "\n";

    // 3. Some hard-coded value (e.g., 1001)
    outFile << QString::number(pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[0])->get_xData_izl().size()).toStdString() + "\n";

    // 4. Timestep
    outFile << QString::number(pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[0])->get_xData_izl()[1] - pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[0])->get_xData_izl()[0] ).toStdString() + "\n";

    // 5. Opis
    outFile << "\"" +setting->getFileDescription().toStdString()+ "\""+"\n";

    //Pocetna vrijednost
    outFile << "0\n";
    //Skala - implementiraj -----------------------
    outFile << "0.001\n";
    //VertPozicija - implementiraj ----------------
    outFile << "400\n";


    // Dodajte dodatne linije potrebne za formatiranje izlaznog fajla


   // Broj redova za ispis
   size_t rowCount = pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport().front())->get_xData_izl().size();

   //OVDJE MOZE BITI PROBLEM ZBOG FORMATA JER NE MORAJU SVI SIGNALI BITI ISTE DUZINE I OBLIKA (HVCB?)
   // Ispis podataka
   for (size_t row = 0; row < rowCount; ++row) {
       // Prva kolona je xDataIzl, samo prvi signal
       outFile << pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport().front())->get_xData_izl(row);


      for (ulong i=0; i<setting->getListaSignalaZaExport().size(); i++)
      {

         outFile << "\t" << pAnsamblSignala->dajSignalPoImenu(setting->getListaSignalaZaExport()[i])->get_yData_izl(row);
      }

       outFile << "\n";
   }

   outFile.close();
}




void SignalsExport::generateOutputExcelFile(const QString& fileName) {
    // Proveri da li ima signala za eksport

    // Kreiraj Excel workbook
    // Otvaranje dijaloga za odabir datoteke slike
    //QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Odaberi Excel fajl"), "", tr("Excel Files (*.xls *.xlsx);;Sve datoteke (*)"));

    if (fileName.isEmpty()) {
        // Korisnik je odabrao otkazivanje dijaloga
        return;
    }

    // Stvori Excel radnu knjigu
    QXlsx::Document workbook(fileName);

    workbook.addSheet("Data");
    workbook.selectSheet("Data");

    // Postavi header kolona
    int colIndex = 1;

    for (ulong i=0; i<pAnsamblSignala->dajVektorSignalaSize(); i++ ) {
        Signall* signal = pAnsamblSignala->dajSignal(i);

        //Potom signale ako su oznaceni za eksport
        if (signal->isMarkedForExport())   //ovo treba svesti na onu listu za eksport
        {
            //Prvo header
            workbook.write(1, colIndex, "time");
            workbook.write(1, colIndex+1, signal->getNewName().toStdString().c_str());

            QVector<double> xvec = signal->get_xData_izl();
            QVector<double> yvec = signal->get_yData_izl();

            for (int row = 0; row < xvec.size() ; ++row) {
                // Prva kolona (xData)
                workbook.write(row+2, colIndex, xvec[row]);
                // Druga kolona (yData)
                workbook.write(row+2, colIndex+1, yvec[row]);
            }

            colIndex = colIndex + 2 ;
        }
    }

    // Spremi fajl
    if (workbook.saveAs(fileName)) {
        QMessageBox::information(this, tr("File Saved"), tr("The file has been saved successfully."));
    } else {
        QMessageBox::warning(this, tr("Save Failed"), tr("The file could not be saved."));
    }
}
