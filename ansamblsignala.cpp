#include "ansamblsignala.h"
#include <QDebug>
#include <memory> // for std::unique_ptr
#include <utility> // for std::move


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{

}


void AnsamblSignala::dodajUAnsambl(std::unique_ptr<Signal> pSignal)
{
    // Premjestiti pSignal u vektor
    vektor_pSignala.push_back(std::move(pSignal));

    // Dohvatiti referencu na posljednji element vektora
    const std::unique_ptr<Signal>& pSignal_novi = vektor_pSignala.back();

    std::cout << "Dodan signal: " << pSignal_novi->ime().toStdString() << std::endl;
}

void AnsamblSignala::ispisiSveSignale()
{
    for (const std::unique_ptr<Signal>& pSignal : vektor_pSignala) {
        qDebug() << pSignal->ime();
    }
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

    //Preslozi nova verzija
    std::sort(vektor_pSignala.begin(), vektor_pSignala.end(), [](const std::unique_ptr<Signal>& a, const std::unique_ptr<Signal>& b) {
        return a->ime() < b->ime();
    });

}
