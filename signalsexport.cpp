#include "signalsexport.h"
#include "ui_signalsexport.h"
#include "exportfilesetting.h"
#include "selectsignalsdialog.h"
#include <QItemSelection>

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

void SignalsExport::on_pushButton_addSignaToFile_clicked()
{
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



}

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
