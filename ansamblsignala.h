#ifndef ANSAMBLSIGNALA_H
#define ANSAMBLSIGNALA_H

#include <QObject>
#include <signal.h>

class AnsamblSignala : public QObject
{
    Q_OBJECT
public:
    explicit AnsamblSignala(QObject *parent = nullptr);

    void dodajUAnsambl(Signal* signal);
    Signal* dajSignal(ulong rbr) {return vektor_pSignala[rbr];}

    std::vector<Signal*> dajVektorSignala(){return vektor_pSignala;}

    void ispisiSveSignale();
    void dodijeliMarkerValueSvimSignalima();
    void presloziVektorSignalaPoAbecedi();

    void ocisti(){vektor_pSignala.clear();} //Ovo treba preko delete sve pojedinacno

signals:
    void changedMarkerValue(double);

private:
    std::vector<Signal*> vektor_pSignala;

};

#endif // ANSAMBLSIGNALA_H
