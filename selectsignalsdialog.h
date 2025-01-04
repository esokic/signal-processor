#ifndef SELECTSIGNALSDIALOG_H
#define SELECTSIGNALSDIALOG_H

#include <QWidget>


#include <QDialog>
#include <QTableWidget>
#include <vector>
#include "signal.h"


namespace Ui {
class SelectSignalsDialog;
}

class SelectSignalsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectSignalsDialog(std::vector<QString> signalNames, QWidget *parent = nullptr);
    ~SelectSignalsDialog();
    std::vector<QString> getSelectedSignalNames() const;

private slots:
    void onSelectionChanged(int row, int column);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::SelectSignalsDialog *ui;
    std::vector<QString> availableSignalNames;
    std::vector<QString> selectedSignalNames; // Čuva selektovane nazive signala
};
#endif // SELECTSIGNALSDIALOG_H
