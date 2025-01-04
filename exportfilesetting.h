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
    QString getExportFileType() const { return exportFileType; }
    //Lista signala se treba vezati za nazive signala a ne za pokazivace na signale!------------------
    //std::vector<Signal*> getListaSignalaZaExport() { return listaSignalaZaExport; }
    std::vector<QString> getListaSignalaZaExport() const {
        return listaSignalaZaExport;
    }

    // Setters
    void setFileType(const QString& value) { fileType = value; }
    void setNazivFileSettingsa(const QString& value) { nazivFileSettingsa = value; }
    void setFileHeader(const QString& value) { fileHeader = value; }
    void setFileDescription(const QString& value) { fileDescription = value; }
    void setNrPhases(const QString& value) { nrPhases = value; }
    void setExportFileType(const QString& value) { exportFileType = value; }
    //Lista signala se treba vezati za nazive signala a ne za pokazivace na signale!------------------
    //void setListaSignalaZaExport(const std::vector<Signal*>& value) { listaSignalaZaExport = value; }
    void setListaSignalaZaExport(const std::vector<QString>& value) {
        listaSignalaZaExport = value;
    }


    //Lista signala se treba vezati za nazive signala a ne za pokazivace na signale!------------------
    //void dodajUListuSignalaZaExport(Signal* signal){listaSignalaZaExport.push_back(signal);}
    void dodajUListuSignalaZaExport(const QString& signalName) {
            // Dodajte signal po imenu (ako nije već u listi)
            if (std::find(listaSignalaZaExport.begin(), listaSignalaZaExport.end(), signalName) == listaSignalaZaExport.end()) {
                listaSignalaZaExport.push_back(signalName);
            }
        }

    void izbrisiElementIzVektoraSignalaZaEksport(ulong currentRow);
    void shiftUpElementIzVektoraSignalaZaEksport(ulong currentRow);
    void shiftDownElementIzVektoraSignalaZaEksport(ulong currentRow);

signals:

private:

    QString fileType = "HVCB";
    QString nazivFileSettingsa = "File setting A";
    QString fileHeader = "Travel, current and arc voltage";
    QString fileDescription = "Description note";
    QString nrPhases = "1ph";
    QString exportFileType = "input";
    //Lista signala se treba vezati za nazive signala a ne za pokazivace na signale!------------------
    //std::vector<Signal*> listaSignalaZaExport;
    std::vector<QString> listaSignalaZaExport;




};

#endif // EXPORTFILESETTING_H
