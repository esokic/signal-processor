#ifndef SIGNALSEXPORT_H
#define SIGNALSEXPORT_H

#include <QWidget>
#include "exportfilesetting.h"
#include "ansamblsignala.h"


namespace Ui {
class SignalsExport;
}

class SignalsExport : public QWidget
{
    Q_OBJECT

public:
    explicit SignalsExport(QWidget *parent = nullptr);
    ~SignalsExport();

public:
    void setPtrAnsamblSignala(AnsamblSignala* ptr){pAnsamblSignala = ptr;}

    //Privremeno
    void exportFileExcel();

private:

    void addNewExportFileSetting(const QString& fileDescription, const QString& fileHeader, const QString& nazivFileSettingsa,
                                     const QString& nrPhases, const QString& exportFileType);

    void populateTableWidget();
    void  populateSignalsTableWidget(ExportFileSetting *currentSetting);

    void onTableWidgetSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void onTableWidgetSettingsSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void updateUIFromVariables(ExportFileSetting& settings);
    void updateVariablesFromUI(ExportFileSetting& settings);


    ExportFileSetting* getCurrentFileSettings();


    void generateOutputHVCBFile_output(const QString& fileName, ExportFileSetting* setting);
    void generateOutputHVCBFile_input(const QString& fileName, ExportFileSetting* setting);
    void generateOutputExcelFile(const QString& fileName);




private slots:
    void on_pushButton_newFileExportSettings_clicked();

    void on_pushButton_updateSettings_clicked();

    void on_pushButton_addSignalToFile_clicked();

    void on_pushButton_removeSignalToFile_clicked();

    void on_pushButton_signalUpList_clicked();

    void on_pushButton_signalDownList_clicked();

    void on_pushButton_exportFiles_clicked();

private:
    Ui::SignalsExport *ui;
    std::vector<ExportFileSetting*> vektorFileSettingsa;

    AnsamblSignala* pAnsamblSignala;
    //std::vector<Signal*> listaSignalaZaExport;
};

#endif // SIGNALSEXPORT_H
