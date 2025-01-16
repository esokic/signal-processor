#include "selectsignalsdialog.h"
#include "ui_selectsignalsdialog.h"
#include <QCheckBox>


SelectSignalsDialog::SelectSignalsDialog(std::vector<QString> signalNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSignalsDialog),
    availableSignalNames(signalNames) {

    ui->setupUi(this);

    // Popuni tabelu sa imenima signala
    ui->tableWidget->setRowCount(signalNames.size());
    ui->tableWidget->setColumnCount(2);
    QStringList headers = {"Select", "Signal Name"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < signalNames.size(); ++i) {
        QTableWidgetItem *nameItem = new QTableWidgetItem(signalNames[i]);
        QCheckBox *checkBox = new QCheckBox(this);
        ui->tableWidget->setItem(i, 1, nameItem);
        ui->tableWidget->setCellWidget(i, 0, checkBox);
    }

    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &SelectSignalsDialog::onSelectionChanged);
}

SelectSignalsDialog::~SelectSignalsDialog()
{
    delete ui;
}



std::vector<QString> SelectSignalsDialog::getSelectedSignalNames() const {
    return selectedSignalNames;
}

void SelectSignalsDialog::onSelectionChanged(int row, int column) {
/*

    if (column == 0) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(row, column));
        if (checkBox && checkBox->isChecked()) {
            selectedSignals.push_back(availableSignals[row]);
        } else {
            selectedSignals.erase(std::remove(selectedSignals.begin(), selectedSignals.end(), availableSignals[row]), selectedSignals.end());
        }
    }
*/
    if (column == 0) { // Ako je promenjena kolona sa checkboxom
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(row, column));
        if (checkBox && checkBox->isChecked()) {
            selectedSignalNames.push_back(availableSignalNames[row]);
        } else {
            selectedSignalNames.erase(std::remove(selectedSignalNames.begin(), selectedSignalNames.end(), availableSignalNames[row]), selectedSignalNames.end());
        }
    }
}

void SelectSignalsDialog::on_pushButton_clicked()
{

    selectedSignalNames.clear();

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(row, 0));
        if (checkBox && checkBox->isChecked()) {
            selectedSignalNames.push_back(availableSignalNames[row]);
        }
    }

    accept(); // Zatvori dijalog nakon selekcije
}

void SelectSignalsDialog::on_pushButton_2_clicked()
{
    reject();
}
