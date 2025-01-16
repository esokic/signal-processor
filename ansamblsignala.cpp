#include "ansamblsignala.h"
#include <QDebug>
#include <memory> // for std::unique_ptr
#include <utility> // for std::move


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{

}


void AnsamblSignala::dodajUAnsambl(std::unique_ptr<Signall> pSignal)
{
    // Premjestiti pSignal u vektor
    vektor_pSignala.push_back(std::move(pSignal));

    // Dohvatiti referencu na posljednji element vektora
    const std::unique_ptr<Signall>& pSignal_novi = vektor_pSignala.back();

    //std::cout << "Dodan signal: " << pSignal_novi->ime().toStdString() << std::endl;
}

Signall* AnsamblSignala::dajSignalPoImenu(const QString& ime)
{
    // Pretražuje vektor i vraća signal sa traženim imenom
    for (const auto& pSignal : vektor_pSignala) {
        if (pSignal->ime() == ime) {
            return pSignal.get();  // Vraća raw pointer na Signal
        }
    }
    return nullptr;  // Ako nije pronađen, vraća nullptr
}

void AnsamblSignala::ispisiSveSignale()
{
    for (const std::unique_ptr<Signall>& pSignal : vektor_pSignala) {
        //qDebug() << pSignal->ime();
    }
}

void AnsamblSignala::dodijeliMarkerValueSvimSignalima()
{
    //Prvo odredi marker value
    double MarkerValue = 0.0;
    for (const std::unique_ptr<Signall>& pSignal : vektor_pSignala) {
        if (pSignal->isMarkerValueAssigned())
        {
            MarkerValue = pSignal->getMarkerValue();
            for (const std::unique_ptr<Signall>& pSignal2 : vektor_pSignala) {
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
    std::sort(vektor_pSignala.begin(), vektor_pSignala.end(), [](const std::unique_ptr<Signall>& a, const std::unique_ptr<Signall>& b) {
        return a->ime() < b->ime();
    });

}
