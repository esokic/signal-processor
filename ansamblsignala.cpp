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
