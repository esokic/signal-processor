#ifndef ANSAMBLSIGNALA_H
#define ANSAMBLSIGNALA_H

#include <QObject>
#include <signal.h>

class AnsamblSignala : public QObject
{
    Q_OBJECT
public:
    explicit AnsamblSignala(QObject *parent = nullptr);

    void dodajUAnsambl(std::shared_ptr<Signal> signal);
    std::shared_ptr<Signal> dajSignal(ulong rbr) {return vektor_pSignala[rbr];}

    std::vector<std::shared_ptr<Signal>> dajVektorSignala(){return vektor_pSignala;}

    void ispisiSveSignale();
    void dodijeliMarkerValueSvimSignalima();
    void presloziVektorSignalaPoAbecedi();

    void ocisti(){vektor_pSignala.clear();} //Ovo treba preko delete sve pojedinacno

    void citajIzMatFajla(const QString& filePath);

signals:
    void changedMarkerValue(double);

private:
    std::vector<std::shared_ptr<Signal>> vektor_pSignala;

};

#endif // ANSAMBLSIGNALA_H
