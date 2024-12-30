#ifndef EXPORTFILESETTING_H
#define EXPORTFILESETTING_H

#include <QObject>
#include <signal.h>

class ExportFileSetting : public QObject
{
    Q_OBJECT
public:
    explicit ExportFileSetting(QObject *parent = nullptr);

    // Getters
    QString getFileType() const { return fileType; }
    QString getNazivFileSettingsa() const { return nazivFileSettingsa; }
    QString getFileHeader() const { return fileHeader; }
    QString getFileDescription() const { return fileDescription; }
    QString getNrPhases() const { return nrPhases; }
    std::vector<Signal*> getListaSignalaZaExport() { return listaSignalaZaExport; }
    QString getExportFileType() const { return exportFileType; }

    // Setters
    void setFileType(const QString& value) { fileType = value; }
    void setNazivFileSettingsa(const QString& value) { nazivFileSettingsa = value; }
    void setFileHeader(const QString& value) { fileHeader = value; }
    void setFileDescription(const QString& value) { fileDescription = value; }
    void setNrPhases(const QString& value) { nrPhases = value; }
    void setListaSignalaZaExport(const std::vector<Signal*>& value) { listaSignalaZaExport = value; }
    void setExportFileType(const QString& value) { exportFileType = value; }

    void dodajUListuSignalaZaExport(Signal* signal){listaSignalaZaExport.push_back(signal);}

    void izbrisiElementIzVektoraSignalaZaEksport(ulong currentRow);
    void shiftUpElementIzVektoraSignalaZaEksport(ulong currentRow);
    void shiftDownElementIzVektoraSignalaZaEksport(ulong currentRow);

signals:

private:

    QString fileType = "HVCB";
    QString nazivFileSettingsa = "File setting A";
    QString fileHeader = "Travel, current and acr voltage";
    QString fileDescription = "Description note";
    QString nrPhases = "1ph";
    std::vector<Signal*> listaSignalaZaExport;
    QString exportFileType = "input";


};

#endif // EXPORTFILESETTING_H
