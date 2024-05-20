#include "selectsignalsdialog.h"
#include "ui_selectsignalsdialog.h"

/*
SelectSignalsDialog::SelectSignalsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectSignalsDialog)
{
    ui->setupUi(this);
}
*/

SelectSignalsDialog::~SelectSignalsDialog()
{
    delete ui;
}


SelectSignalsDialog::SelectSignalsDialog(const std::vector<Signal*>& signali, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSignalsDialog),
    availableSignals(signali) {

        ui->setupUi(this);

        // Popuni tabelu signalima
        ui->tableWidget->setRowCount(signali.size());
        ui->tableWidget->setColumnCount(2);
        QStringList headers = {"Select", "Signal Name"};
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        for (size_t i = 0; i < signali.size(); ++i) {
            QTableWidgetItem *nameItem = new QTableWidgetItem(signali[i]->ime());
            QCheckBox *checkBox = new QCheckBox(this);
            ui->tableWidget->setItem(i, 1, nameItem);
            ui->tableWidget->setCellWidget(i, 0, checkBox);
    }

    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &SelectSignalsDialog::onSelectionChanged);
}


std::vector<Signal*> SelectSignalsDialog::getSelectedSignals() const {
    return selectedSignals;
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
}

void SelectSignalsDialog::on_pushButton_clicked()
{


    selectedSignals.clear();
    /*
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked) {
            selectedSignals.push_back(availableSignals[i]);
        }
    }*/

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(row, 0));
        if (checkBox && checkBox->isChecked()) {
            selectedSignals.push_back(availableSignals[row]);
        } else {
            selectedSignals.erase(std::remove(selectedSignals.begin(), selectedSignals.end(), availableSignals[row]), selectedSignals.end());
        }

    }





    accept();
}

void SelectSignalsDialog::on_pushButton_2_clicked()
{
    reject();
}
