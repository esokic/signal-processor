#ifndef ANSAMBLSIGNALA_H
#define ANSAMBLSIGNALA_H

#include <QObject>
#include <signal.h>
#include <manipulacijaprocesorima.h>

class AnsamblSignala : public QObject
{
    Q_OBJECT
public:
    explicit AnsamblSignala(QObject *parent = nullptr);

    //Setteri
    void dodajUAnsambl(std::unique_ptr<Signal> signal);
    void ocisti(){
        vektor_pSignala.clear(); // Očisti vektor
    }

    void ocistiVisakSignala(QStringList listaNaziva)
    {
    vektor_pSignala.erase(std::remove_if(vektor_pSignala.begin(), vektor_pSignala.end(),
        [&listaNaziva](const std::unique_ptr<Signal>& signal) {
            return !listaNaziva.contains(signal->ime());
        }),
        vektor_pSignala.end());
    }
    //Getteri

    Signal* dajSignal(ulong rbr) {         //Vraca pokazivac na Signal (iz vektora)
        if (rbr >= vektor_pSignala.size()) {
            throw std::out_of_range("Indeks signala je izvan granica.");
        }
        return vektor_pSignala[rbr].get();
    }

    //Vraca pokazivac na signal po imenu
    Signal* dajSignalPoImenu(const QString& ime);

    ulong dajVektorSignalaSize(){return vektor_pSignala.size();}


    void ispisiSveSignale();
    void dodijeliMarkerValueSvimSignalima();
    void presloziVektorSignalaPoAbecedi();

    void setPointerNaManipulacijuProcesorima(ManipulacijaProcesorima* _ptr){manProc = _ptr;}

signals:
    void changedMarkerValue(double);

private:
    std::vector<std::unique_ptr<Signal>> vektor_pSignala;
    ManipulacijaProcesorima* manProc;

};

#endif // ANSAMBLSIGNALA_H
