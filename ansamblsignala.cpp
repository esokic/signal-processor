#include "ansamblsignala.h"
#include <QDebug>
#include <memory> // for std::unique_ptr
#include <utility> // for std::move


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{
    /*
    // Inicijalizacija vektora u konstruktoru
    vektor_pSignala = std::vector<std::unique_ptr<Signal>>();
    //Dummy signal
    Signal mojSignal;
    mojSignal.setObjectName("test");
    Signal* pMojSignal = new Signal();
    std::unique_ptr<Signal> unique_ptr_signal(pMojSignal); // Pretvaranje ptr u std::unique_ptr<Signal>
    //Sad se vektor brine o njemu
    vektor_pSignala.push_back(std::move(unique_ptr_signal));
    */
    //Dummy signal
    /*
    Signal mojSignal;
    mojSignal.setObjectName("test");
    std::unique_ptr<Signal> unique_ptr_signal = std::make_unique<Signal>();
    vektor_pSignala.emplace_back(std::move(unique_ptr_signal));
    */
}


void AnsamblSignala::dodajUAnsambl(std::unique_ptr<Signal> pSignal)
{
    //Za pocetak dodaj mu genericki Procesor
    //pSignal->setPointerNaProcesor(manProc->getPointerNaDefaultniProcesor());
    vektor_pSignala.push_back(std::move(pSignal));

}

void AnsamblSignala::ispisiSveSignale()
{
    /*
    for (const std::unique_ptr<Signal>& pSignal : vektor_pSignala) {
            qDebug() << pSignal->ime() ;//<< pSignal->ispisi_dimenzije();
    }
    */
}

void AnsamblSignala::dodijeliMarkerValueSvimSignalima()
{
    //Prvo odredi marker value
    double MarkerValue = 0.0;
    for (const std::unique_ptr<Signal>& pSignal : vektor_pSignala) {
        if (pSignal->isMarkerValueAssigned())
        {
            MarkerValue = pSignal->getMarkerValue();
            for (const std::unique_ptr<Signal>& pSignal2 : vektor_pSignala) {
                //Sada svima dodijeli
                if (!(pSignal2->isMarkerValueAssigned())) {pSignal2->setMarkerValue(MarkerValue);}
            }
            emit changedMarkerValue(MarkerValue);
            break;
        }
    }

}

void AnsamblSignala::presloziVektorSignalaPoAbecedi()
{
   /*
    //Preslozi
    // Popunjavanje tabele signala
    auto vektorSignala = vektor_pSignala;

    // Poredajte vektor signala po abecednom redu prema imenu
    std::sort(vektorSignala.begin(), vektorSignala.end(), [](Signal* a, Signal* b) {
        return a->ime() < b->ime();
    });

    vektor_pSignala = vektorSignala;
    */
}
