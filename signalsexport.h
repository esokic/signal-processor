#ifndef SIGNALSEXPORT_H
#define SIGNALSEXPORT_H

#include <QWidget>
#include "exportfilesetting.h"

namespace Ui {
class SignalsExport;
}

class SignalsExport : public QWidget
{
    Q_OBJECT

public:
    explicit SignalsExport(QWidget *parent = nullptr);
    ~SignalsExport();

    void addNewExportFileSetting(const QString& fileDescription, const QString& fileHeader, const QString& nazivFileSettingsa,
                                     const QString& nrPhases, const QString& exportFileType);

    void populateTableWidget();
    void onTableWidgetSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void updateUIFromVariables(ExportFileSetting& settings);
    void updateVariablesFromUI(ExportFileSetting& settings);

private slots:
    void on_pushButton_newFileExportSettings_clicked();

    void on_pushButton_updateSettings_clicked();

private:
    Ui::SignalsExport *ui;

    std::vector<ExportFileSetting*> vektorFileSettingsa;
};

#endif // SIGNALSEXPORT_H
