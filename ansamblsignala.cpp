#include "ansamblsignala.h"
#include <QDebug>


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{

}

void AnsamblSignala::dodajUAnsambl(Signal *pSignal)
{
    vektor_pSignala.push_back(pSignal);
}

void AnsamblSignala::ispisiSveSignale()
{
    for (Signal *pSignal : vektor_pSignala) {
            qDebug() << pSignal->ime() ;//<< pSignal->ispisi_dimenzije();
    }
}

void AnsamblSignala::dodijeliMarkerValueSvimSignalima()
{
    //Prvo odredi marker value
    double MarkerValue = 0.0;
    for (Signal *pSignal : vektor_pSignala) {
        if (pSignal->isMarkerValueAssigned())
        {
            MarkerValue = pSignal->getMarkerValue();
            for (Signal *pSignal2 : vektor_pSignala) {
                //Sada svima dodijeli
                if (!(pSignal2->isMarkerValueAssigned())) {pSignal2->setMarkerValue(MarkerValue);}
            }
            emit changedMarkerValue(MarkerValue);
            break;
        }
    }

}
