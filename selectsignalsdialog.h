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
    explicit SelectSignalsDialog(std::vector<std::unique_ptr<Signal>>& signali, QWidget *parent = nullptr);
    ~SelectSignalsDialog();
    std::vector<std::unique_ptr<Signal> > getSelectedSignals() const;

private slots:
    void onSelectionChanged(int row, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SelectSignalsDialog *ui;
    std::vector<std::unique_ptr<Signal>> availableSignals;
    std::vector<std::unique_ptr<Signal>> selectedSignals;
};

#endif // SELECTSIGNALSDIALOG_H
