#include "manipulacijaprocesorima.h"
#include "ui_manipulacijaprocesorima.h"
#include <QDebug>

ManipulacijaProcesorima::ManipulacijaProcesorima(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManipulacijaProcesorima)
{
    ui->setupUi(this);

    //Generisi defaultni procesor
    Procesor *defProc = new Procesor;
    defProc->setIme("------");
    pDefaultniProcesor = defProc;

    // Postavljanje broja redova i kolona

    ui->tableWidget_tabelaProcesora->setRowCount(static_cast<int>(vektorProcesora.size()));
    ui->tableWidget_tabelaProcesora->setColumnCount(2);
    QStringList headers;
    headers << "Processor name" << "Description";
    ui->tableWidget_tabelaProcesora->setHorizontalHeaderLabels(headers);
    //Da se moze odabrati samo jedan red:
    ui->tableWidget_tabelaProcesora->setSelectionMode(QAbstractItemView::SingleSelection);

    // Popunjavanje QTableWidget-a
    populateTableWidget();

    // Povezivanje signala za svaki procesor sa slotom za osvježavanje tabele
    for (auto *procesor : vektorProcesora) {
        connect(procesor, &Procesor::signalOsvjezi, this, &ManipulacijaProcesorima::onProcessorOsvjezi);
    }

    connect(ui->tableWidget_tabelaProcesora, &QTableWidget::itemSelectionChanged, this, &ManipulacijaProcesorima::onItemSelectionChanged);
    connect(ui->tableWidget_tabelaProcesora, &QTableWidget::cellDoubleClicked, this, &ManipulacijaProcesorima::onTableItemDoubleClicked);
}

ManipulacijaProcesorima::~ManipulacijaProcesorima()
{
    delete ui;
}

void ManipulacijaProcesorima::populateTableWidget() {

    ui->tableWidget_tabelaProcesora->clearContents();
    ui->tableWidget_tabelaProcesora->setRowCount(static_cast<int>(vektorProcesora.size()));
    for (size_t row = 0; row < vektorProcesora.size(); ++row) {
        const auto &procesor = vektorProcesora[row];
        QTableWidgetItem *nameItem = new QTableWidgetItem(procesor->getIme());
        QTableWidgetItem *descriptionItem = new QTableWidgetItem(procesor->getOpis());
        ui->tableWidget_tabelaProcesora->setItem(static_cast<int>(row), 0, nameItem);
        ui->tableWidget_tabelaProcesora->setItem(static_cast<int>(row), 1, descriptionItem);
    }
}

void ManipulacijaProcesorima::onItemSelectionChanged() {
    /*
    QTableWidgetItem *item = ui->tableWidget_tabelaProcesora->currentItem();
    if (item) {
        // Do something with the selected item
        qDebug() << "Selected item:" << item->text();
    }
    */

    // Dobijte listu odabranih indeksa u tabeli
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget_tabelaProcesora->selectedItems();

    // Provjerite da li je izabran tačno jedan red
    if (selectedItems.size() != 1) {
        // Ako nije izabran tačno jedan red, poništite odabir i izađite iz metode
        ui->tableWidget_tabelaProcesora->clearSelection();
        return;
    }

    // Dobijte redni broj izabranog reda
    int selectedRow = selectedItems[0]->row();

    // Provjerite da li je redni broj u granicama vektora
    if (selectedRow < 0 || selectedRow >= static_cast<int>(vektorProcesora.size())) {
        // Ako nije, poništite odabir i izađite iz metode
        ui->tableWidget_tabelaProcesora->clearSelection();
        return;
    }

    // Dobijte objekat Procesor iz vektora na osnovu rednog broja
    pOdabraniProcesor = vektorProcesora[static_cast<ulong>(selectedRow)];

    // Sada možete raditi šta god želite sa odabranim objektom Procesor
    qDebug() << "Selected Procesor: " << pOdabraniProcesor->getIme() << ", " << pOdabraniProcesor->getOpis();
}


void ManipulacijaProcesorima::on_pushButton_newProcessor_clicked()
{
    Procesor *procesor = new Procesor;
    //procesor->setIme("test" + QString::number(i));
    //procesor->setOpis("opis" + QString::number(i));
    dodajProcesor(procesor);
    procesor->setStartTimeFromMarkerValue(MarkerValue);
    // Povežite signal sa slotom
    connect(procesor, &Procesor::signalOsvjezi, this, &ManipulacijaProcesorima::onProcessorOsvjezi);
    // Osvježite tabelu da uključite novi procesor:
    populateTableWidget();

    procesor->show();

    i++;

}

void ManipulacijaProcesorima::onTableItemDoubleClicked(int row, int column) {

    // Provjerite da li je redni broj u granicama vektora
    if (row < 0 || row >= static_cast<int>(vektorProcesora.size())) {
        // Ako nije, poništite odabir i izađite iz metode
        ui->tableWidget_tabelaProcesora->clearSelection();
        return;
    }

    // Dobijte objekat Procesor iz vektora na osnovu rednog broja
    pOdabraniProcesor = vektorProcesora[static_cast<ulong>(row)];

    pOdabraniProcesor->prikaziGUI();
}


void ManipulacijaProcesorima::onProcessorOsvjezi()
{
    //Osvjezi tabelu
    populateTableWidget();
}

void ManipulacijaProcesorima::on_pushButton_editProcessor_clicked()
{
    pOdabraniProcesor->prikaziGUI();
}
