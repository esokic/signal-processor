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
        for (auto& ptr : vektor_pSignala) {
                ptr.reset(); // Oslobađa memoriju na koju `ptr` pokazuje
            }
        vektor_pSignala.clear(); // Očisti vektor
    } //Ovo treba preko delete sve pojedinacno

    //Getteri
    const std::vector<std::unique_ptr<Signal>>& dajVektorSignala() const {
        return vektor_pSignala;
    }

    /*
    std::vector<Signal*> dajVektorSignala(){  //Ovo treba ukinuti u principu
        Signal* pSig;
        std::vector<Signal*> vec;
        for (ulong i=0; i<vektor_pSignala.size(); i++)
        {
            pSig = vektor_pSignala[i].get();
            vec.push_back(pSig);
        }
        return vec;}
*/
    Signal* dajSignal(ulong rbr) {         //Vraca pokazivac
        return vektor_pSignala[rbr].get();
    }

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
