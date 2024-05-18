#include "manipulacijaprocesorima.h"
#include "ui_manipulacijaprocesorima.h"

ManipulacijaProcesorima::ManipulacijaProcesorima(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManipulacijaProcesorima)
{
    ui->setupUi(this);

    // Postavljanje broja redova i kolona
    ui->tableWidget_tabelaProcesora->setRowCount(30);
    ui->tableWidget_tabelaProcesora->setColumnCount(2);
    QStringList headers;
    headers << "Processor name" << "Description";
    ui->tableWidget_tabelaProcesora->setHorizontalHeaderLabels(headers);
    //Da se moze odabrati samo jedan red:
    ui->tableWidget_tabelaProcesora->setSelectionMode(QAbstractItemView::SingleSelection);

    // Popunjavanje QTableWidget-a
    populateTableWidget();

    connect(ui->tableWidget_tabelaProcesora, &QTableWidget::itemSelectionChanged, this, &ManipulacijaProcesorima::onItemSelectionChanged);
}

ManipulacijaProcesorima::~ManipulacijaProcesorima()
{
    delete ui;
}

void ManipulacijaProcesorima::populateTableWidget() {
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
    procesor->setIme("test" + QString::number(i));
    procesor->setOpis("opis" + QString::number(i));
    dodajProcesor(procesor);
    procesor->show();

    i++;

}
