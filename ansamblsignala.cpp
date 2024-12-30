#include "ansamblsignala.h"
#include <QDebug>
#include <memory> // for std::unique_ptr
#include <utility> // for std::move


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{
    /*
    // Inicijalizacija vektora u konstruktoru
    vektor_pSignala = std::vector<Signal*>();
    //Dummy signal
    Signal mojSignal;
    mojSignal.setObjectName("test");
    Signal* pMojSignal = new Signal();
    Signal* unique_ptr_signal(pMojSignal); // Pretvaranje ptr u Signal*
    //Sad se vektor brine o njemu
    vektor_pSignala.push_back(std::move(unique_ptr_signal));
    */
    //Dummy signal
    /*
    Signal mojSignal;
    mojSignal.setObjectName("test");
    Signal* unique_ptr_signal = std::make_unique<Signal>();
    vektor_pSignala.emplace_back(std::move(unique_ptr_signal));
    */
}


void AnsamblSignala::dodajUAnsambl(Signal* pSignal)
{

    //Za pocetak dodaj mu genericki Procesor
    //pSignal->setPointerNaProcesor(manProc->getPointerNaDefaultniProcesor());
    vektor_pSignala.push_back(std::move(pSignal));


    //vektor_pSignala.push_back(pSignal);
    std::cout << "Dodan signal:" << pSignal->ime().toStdString();

}

void AnsamblSignala::ispisiSveSignale()
{
    /*
    for (const Signal*& pSignal : vektor_pSignala) {
            qDebug() << pSignal->ime() ;//<< pSignal->ispisi_dimenzije();
    }
    */
}

void AnsamblSignala::dodijeliMarkerValueSvimSignalima()
{
    //Prvo odredi marker value
    double MarkerValue = 0.0;
    for (Signal*& pSignal : vektor_pSignala) {
        if (pSignal->isMarkerValueAssigned())
        {
            MarkerValue = pSignal->getMarkerValue();
            for (Signal*& pSignal2 : vektor_pSignala) {
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
