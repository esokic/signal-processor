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
    void dodajUAnsambl(Signal* signal);
    void ocisti(){
        vektor_pSignala.clear(); // Očisti vektor
    }

    //Getteri
    std::vector<Signal*> dajVektorSignala() {
        return vektor_pSignala;
    }

    Signal* dajSignal(ulong rbr) {         //Vraca pokazivac
        return vektor_pSignala[rbr];
    }

    ulong dajVektorSignalaSize(){return vektor_pSignala.size();}


    void ispisiSveSignale();
    void dodijeliMarkerValueSvimSignalima();
    void presloziVektorSignalaPoAbecedi();

    void setPointerNaManipulacijuProcesorima(ManipulacijaProcesorima* _ptr){manProc = _ptr;}

signals:
    void changedMarkerValue(double);

private:
    std::vector<Signal*> vektor_pSignala;
    ManipulacijaProcesorima* manProc;

};

#endif // ANSAMBLSIGNALA_H
